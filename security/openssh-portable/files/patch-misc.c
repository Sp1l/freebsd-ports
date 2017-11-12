------------------------------------------------------------------------
r181918 | des | 2008-08-20 05:40:07 -0500 (Wed, 20 Aug 2008) | 6 lines
Changed paths:
   M /head/crypto/openssh/readconf.c

Use net.inet.ip.portrange.reservedhigh instead of IPPORT_RESERVED.
Submitted upstream, no reaction.

Submitted by:   delphij@
[rewritten for 7.4 by bdrewery@]

--- misc.c.orig	2017-10-02 19:34:26 UTC
+++ misc.c
@@ -63,6 +63,8 @@
 #include <net/if.h>
 #endif
 
+#include <sys/sysctl.h>
+
 #include "xmalloc.h"
 #include "misc.h"
 #include "log.h"
@@ -1259,11 +1261,26 @@ forward_equals(const struct Forward *a, 
 	return 1;
 }
 
+static int
+ipport_reserved(void)
+{
+#if __FreeBSD__
+	int old, ret;
+	size_t len = sizeof(old);
+
+	ret = sysctlbyname("net.inet.ip.portrange.reservedhigh",
+	    &old, &len, NULL, 0);
+	if (ret == 0)
+		return (old + 1);
+#endif
+	return (IPPORT_RESERVED);
+}
+
 /* returns 1 if bind to specified port by specified user is permitted */
 int
 bind_permitted(int port, uid_t uid)
 {
-	if (port < IPPORT_RESERVED && uid != 0)
+	if (port < ipport_reserved() && uid != 0)
 		return 0;
 	return 1;
 }
