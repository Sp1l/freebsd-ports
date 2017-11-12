------------------------------------------------------------------------
r99055 | des | 2002-06-29 04:21:58 -0700 (Sat, 29 Jun 2002) | 6 lines
Changed paths:
   M /head/crypto/openssh/session.c

Make sure the environment variables set by setusercontext() are passed on
to the child process.

Reviewed by:    ache
Sponsored by:   DARPA, NAI Labs


--- session.c.orig	2017-10-02 19:34:26 UTC
+++ session.c
@@ -966,6 +966,9 @@ do_setup_env(struct ssh *ssh, Session *s
 	struct passwd *pw = s->pw;
 #if !defined (HAVE_LOGIN_CAP) && !defined (HAVE_CYGWIN)
 	char *path = NULL;
+#else
+	extern char **environ;
+	char **senv, **var, *val;
 #endif
 
 	/* Initialize the environment. */
@@ -987,6 +990,9 @@ do_setup_env(struct ssh *ssh, Session *s
 	}
 #endif
 
+	if (getenv("TZ"))
+		child_set_env(&env, &envsize, "TZ", getenv("TZ"));
+
 #ifdef GSSAPI
 	/* Allow any GSSAPI methods that we've used to alter
 	 * the childs environment as they see fit
@@ -1004,11 +1010,30 @@ do_setup_env(struct ssh *ssh, Session *s
 	child_set_env(&env, &envsize, "LOGIN", pw->pw_name);
 #endif
 	child_set_env(&env, &envsize, "HOME", pw->pw_dir);
+	snprintf(buf, sizeof buf, "%.200s/%.50s", _PATH_MAILDIR, pw->pw_name);
+	child_set_env(&env, &envsize, "MAIL", buf);
 #ifdef HAVE_LOGIN_CAP
-	if (setusercontext(lc, pw, pw->pw_uid, LOGIN_SETPATH) < 0)
-		child_set_env(&env, &envsize, "PATH", _PATH_STDPATH);
-	else
-		child_set_env(&env, &envsize, "PATH", getenv("PATH"));
+	child_set_env(&env, &envsize, "PATH", _PATH_STDPATH);
+	child_set_env(&env, &envsize, "TERM", "su");
+	/*
+	 * Temporarily swap out our real environment with an empty one,
+	 * let setusercontext() apply any environment variables defined
+	 * for the user's login class, copy those variables to the child,
+	 * free the temporary environment, and restore the original.
+	 */
+	senv = environ;
+	environ = xmalloc(sizeof(*environ));
+	*environ = NULL;
+	(void)setusercontext(lc, pw, pw->pw_uid, LOGIN_SETENV|LOGIN_SETPATH);
+	for (var = environ; *var != NULL; ++var) {
+		if ((val = strchr(*var, '=')) != NULL) {
+			*val++ = '\0';
+			child_set_env(&env, &envsize, *var, val);
+		}
+		free(*var);
+	}
+	free(environ);
+	environ = senv;
 #else /* HAVE_LOGIN_CAP */
 # ifndef HAVE_CYGWIN
 	/*
@@ -1028,15 +1053,9 @@ do_setup_env(struct ssh *ssh, Session *s
 # endif /* HAVE_CYGWIN */
 #endif /* HAVE_LOGIN_CAP */
 
-	snprintf(buf, sizeof buf, "%.200s/%.50s", _PATH_MAILDIR, pw->pw_name);
-	child_set_env(&env, &envsize, "MAIL", buf);
-
 	/* Normal systems set SHELL by default. */
 	child_set_env(&env, &envsize, "SHELL", shell);
 
-	if (getenv("TZ"))
-		child_set_env(&env, &envsize, "TZ", getenv("TZ"));
-
 	/* Set custom environment options from RSA authentication. */
 	while (custom_environment) {
 		struct envstring *ce = custom_environment;
@@ -1321,7 +1340,7 @@ do_setusercontext(struct passwd *pw)
 	if (platform_privileged_uidswap()) {
 #ifdef HAVE_LOGIN_CAP
 		if (setusercontext(lc, pw, pw->pw_uid,
-		    (LOGIN_SETALL & ~(LOGIN_SETPATH|LOGIN_SETUSER))) < 0) {
+		    (LOGIN_SETALL & ~(LOGIN_SETENV|LOGIN_SETPATH|LOGIN_SETUSER))) < 0) {
 			perror("unable to set user context");
 			exit(1);
 		}
