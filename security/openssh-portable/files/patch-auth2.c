--- UTC
r99053 | des | 2002-06-29 05:57:13 -0500 (Sat, 29 Jun 2002) | 4 lines
Changed paths:
   M /head/crypto/openssh/auth2.c

Apply class-imposed login restrictions.

--- auth2.c.orig	2017-10-02 19:34:26 UTC
+++ auth2.c
@@ -221,6 +221,11 @@ input_userauth_request(int type, u_int32
 	Authmethod *m = NULL;
 	char *user, *service, *method, *style = NULL;
 	int authenticated = 0;
+#ifdef HAVE_LOGIN_CAP
+	login_cap_t *lc;
+	const char *from_host, *from_ip;
+
+#endif
 
 	if (authctxt == NULL)
 		fatal("input_userauth_request: no authctxt");
@@ -270,6 +275,28 @@ input_userauth_request(int type, u_int32
 		    "(%s,%s) -> (%s,%s)",
 		    authctxt->user, authctxt->service, user, service);
 	}
+
+#ifdef HAVE_LOGIN_CAP
+	if (authctxt->pw != NULL &&
+	    (lc = login_getpwclass(authctxt->pw)) != NULL) {
+		logit("user %s login class %s", authctxt->pw->pw_name,
+		    authctxt->pw->pw_class);
+		from_host = auth_get_canonical_hostname(ssh, options.use_dns);
+		from_ip = ssh_remote_ipaddr(ssh);
+		if (!auth_hostok(lc, from_host, from_ip)) {
+			logit("Denied connection for %.200s from %.200s [%.200s].",
+			    authctxt->pw->pw_name, from_host, from_ip);
+			packet_disconnect("Sorry, you are not allowed to connect.");
+		}
+		if (!auth_timeok(lc, time(NULL))) {
+			logit("LOGIN %.200s REFUSED (TIME) FROM %.200s",
+			    authctxt->pw->pw_name, from_host);
+			packet_disconnect("Logins not available right now.");
+		}
+		login_close(lc);
+	}
+#endif  /* HAVE_LOGIN_CAP */
+
 	/* reset state */
 	auth2_challenge_stop(ssh);
 
