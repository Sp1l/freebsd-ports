# $FreeBSD: head/www/apache24/Makefile 452732 2017-10-23 18:49:27Z brnrd $

PORTNAME=	apache24
PORTVERSION=	2.4.29
CATEGORIES=	www ipv6
MASTER_SITES=	APACHE_HTTPD
DISTNAME=	httpd-${PORTVERSION}
DIST_SUBDIR=	apache24

MAINTAINER=	apache@FreeBSD.org
COMMENT=	Version 2.4.x of Apache web server

LICENSE=	APACHE20
LICENSE_FILE=	${WRKSRC}/LICENSE

LIB_DEPENDS=	libexpat.so:textproc/expat2 \
		libapr-1.so:devel/apr1 \
		libpcre.so:devel/pcre

CONFLICTS_INSTALL=	caudium14-1.* \
			apache-*-2.2.* apache22-*

USE_APACHE=	common24
USES=		autoreconf cpe iconv libtool perl5 tar:bzip2
USE_PERL5=	run
USE_RC_SUBR=	apache24 htcacheclean
GNU_CONFIGURE=	yes

CPE_VENDOR=	apache
CPE_PRODUCT=	http_server

PORTDOCS=	*
SUB_FILES=	pkg-install pkg-deinstall

# XXX: before running makepatch please run the command
# `$SED -e 's/PATCH_PATH_SEPARATOR=/PATCH_PATH_SEPARATOR?=/' Mk/bsd.port.mk
PATCH_PATH_SEPARATOR=	__

# Fallback MPM after switching from static to modular MPM
SUB_LIST+=	MPMF="000_mpm_prefork_fallback.conf"

USERS=		www
GROUPS=		www

APACHEDIR=	${.CURDIR}

WITH_HTTP_PORT?=	80
WITH_SSL_PORT?=		443

.if !defined(WITH_DEBUG)
EXTRA_PATCHES+=	${FILESDIR}/extra-patch-server_buildmark.c
.endif

.include "Makefile.options"
.include "Makefile.options.desc"
OPTIONS_SUB=	yes

# IMPLIES
AUTH_BASIC_IMPLIES=		AUTHN_CORE
AUTH_DIGEST_IMPLIES=		AUTHN_CORE AUTHZ_CORE
AUTHN_DBD_IMPLIES=		DBD
HEARTBEAT_IMPLIES=		WATCHDOG STATUS
HEARTMONITOR_IMPLIES=		WATCHDOG STATUS
LBMETHOD_HEARTBEAT_IMPLIES=	WATCHDOG STATUS HEARTMONITOR
PROXY_HCHECK_IMPLIES=		WATCHDOG
PROXY_HTTP2_IMPLIES=		PROXY_BALANCER

AUTHNZ_LDAP_USE=		openldap=yes
BROTLI_CONFIGURE_WITH=		brotli=${LOCALBASE}
BROTLI_LIB_DEPENDS=		libbrotlicommon.so:archivers/brotli
HTTP2_CONFIGURE_WITH=		nghttp2=${LOCALBASE}
HTTP2_LIB_DEPENDS=		libnghttp2.so:www/libnghttp2
# http://httpd.apache.org/docs/2.4/bind.html
IPV4_MAPPED_CONFIGURE_ENABLE=	v4-mapped
LDAP_USE=			openldap=yes
LUAJIT_LIB_DEPENDS=		libluajit-5.1.so:lang/luajit
LUA_CONFIGURE_ENV=		LUA_CFLAGS="-I${LUA_INCDIR}" \
				LUA_LIBS="-L${LUA_LIBDIR} -llua-${LUA_VER}"
LUA_CONFIGURE_WITH=		lua=${PREFIX}
LUA_USES=			lua
MPM_PREFORK_CONFIGURE_ON=	--with-mpm=prefork
MPM_WORKER_CONFIGURE_ON=	--with-mpm=worker
MPM_EVENT_CONFIGURE_ON=		--with-mpm=event
MPM_SHARED_CONFIGURE_ON=	--enable-mpms-shared=all
PROXY_HTML_USE=			gnnome=libxml2
PROXY_HTML_WITH=		libxml2=${LOCALBASE}/include/libxml2
PROXY_HTTP2_CONFIGURE_WITH=	nghttp2=${LOCALBASE}
PROXY_HTTP2_LIB_DEPENDS=	libnghttp2.so:www/libnghttp2
SOCACHE_DC_CONFIGURE_WITH=	distcache=${LOCALBASE}
SOCACHE_DC_LIB_DEPENDS=		libdistcache.so:security/distcache
SSL_CONFIGURE_WITH=		ssl=${OPENSSLBASE}
SSL_USES=			ssl
XML2ENC_USE=			gnome=libxml2
XML2ENC_WITH=			libxml2=${LOCALBASE}/include/libxml2
.for mod in ${EXAMPLE_DISABLED_MODULES}
${mod}_VARS=                    with-devmods=yes
.endfor

ETC_SUBDIRS=		Includes envvars.d extra modules.d

APR_CONFIG?=		${LOCALBASE}/bin/apr-1-config
APU_CONFIG?=		${LOCALBASE}/bin/apu-1-config
# APU module used by AUTHNZ_LDAP LDAP
APU_LDAP?=		${LOCALBASE}/lib/apr-util-1/apr_ldap.so
# APU module used by SESSION_CRYPTO
APU_CRYPTO_OPENSSL?=	${LOCALBASE}/lib/apr-util-1/apr_crypto_openssl.so
APU_CRYPTO_NSS?=	${LOCALBASE}/lib/apr-util-1/apr_crypto_nss.so

PREFIX_RELDEST=	${PREFIX:S,^${DESTDIR},,}

CONFIGURE_ARGS+=--prefix=${PREFIX_RELDEST} \
		--enable-layout=FreeBSD \
		--with-port=${WITH_HTTP_PORT} \
		--with-sslport=${WITH_SSL_PORT} \
		--with-expat=${LOCALBASE} \
		--with-iconv=${ICONV_PREFIX} \
		--enable-http \
		--with-pcre=${LOCALBASE} \
		--with-apr=${APR_CONFIG} \
		--with-apr-util=${APU_CONFIG}

CONFIGURE_ENV=	LOCALBASE="${LOCALBASE}" \
		CONFIG_SHELL="${SH}"

MAKE_ENV+=	EXPR_COMPAT=yes \
		INSTALL_MAN="${INSTALL_MAN}" \
		DATADIR=${DATADIR}

.include <bsd.port.pre.mk>
.include "Makefile.modules"

.if ${OPSYS} == FreeBSD && ${OSVERSION} < 1100085 &&\
	${PORT_OPTIONS:MHTTP2} && ${OPENSSLBASE} == /usr
SUB_FILES+=    pkg-message
.endif

post-extract:
# remove possible leftover .svn directories in the sources
	@${FIND} ${WRKSRC} -type d -name .svn -print | ${XARGS} ${RM} -r
# limit grep results ...
	@${FIND} ${WRKSRC} -type f \( -name 'NWGNU*' -o -name '*.ds?' -o -name '*.dep' -o -name '*.mak' -o -name '*.win' -o -name '*.vbs' -o -name '*.wsf' \) -delete
# make sure the configure script contains our patches, preserve the original script for comparsion
	-${MV} -v ${WRKSRC}/configure ${WRKSRC}/configure.upstream

# make stage-qa script happy, it complains on empty dirs even 'PORTDOCS=*' is set
# use RMDIR in case upstream ever place some files into this directories
.for d in xsl/util xsl lang
	-${RMDIR} ${WRKSRC}/docs/manual/style/${d}
.endfor

post-patch:
	${REINPLACE_CMD} -e 's," PLATFORM ",FreeBSD,' ${WRKSRC}/server/core.c
	${REINPLACE_CMD} -e 's|logs/error_log|/var/log/httpd-error.log|' \
		${WRKSRC}/include/httpd.h
	${REINPLACE_CMD} -e 's|perlbin=.*|perlbin=${PERL}|' \
		${WRKSRC}/configure.in
	${RM} ${WRKSRC}/docs/docroot/*.bak
	${INSTALL_DATA} ${WRKSRC}/NOTICE ${WRKSRC}/docs/manual

pre-configure::
	@${ECHO_MSG}	""
	@${ECHO_MSG}	"  You can check your modules configuration by using make show-modules"
	@${ECHO_MSG}	""
# silence autotools
	-${MV} -v ${WRKSRC}/configure.in ${WRKSRC}/configure.ac

post-configure:
	@FTPUSERS=`${EGREP} -v '^#' /etc/ftpusers| ${TR} -s "\n" " "` ;\
		${REINPLACE_CMD} -e "s,%%FTPUSERS%%,$${FTPUSERS}," \
			${WRKSRC}/docs/conf/extra/httpd-userdir.conf
	${REINPLACE_CMD} -e "s,%%WWWOWN%%,${WWWOWN}," -e "s,%%WWWGRP%%,${WWWGRP}," \
		${WRKSRC}/docs/conf/httpd.conf
	${REINPLACE_CMD} -e "s,%%PREFIX%%,${PREFIX}," ${WRKSRC}/support/envvars-std

post-install:
	@${MKDIR} ${ETC_SUBDIRS:S|^|${STAGEDIR}${ETCDIR}/|}
	${INSTALL_DATA} ${FILESDIR}/no-accf.conf ${STAGEDIR}${ETCDIR}/Includes/
# place for 3rd party module configuration
	${INSTALL_DATA} ${FILESDIR}/README_modules.d ${STAGEDIR}${ETCDIR}/modules.d/
# strip returns an error for non binary files, but we have a big mix
	-${STRIP_CMD} ${STAGEDIR}${PREFIX}/sbin/* 2>/dev/null
	-${STRIP_CMD} ${STAGEDIR}${PREFIX}/bin/*
	-${STRIP_CMD} ${STAGEDIR}${PREFIX}/libexec/apache24/mod_*.so

post-install-LOG_FORENSIC-on:
	${INSTALL_SCRIPT} ${WRKSRC}/support/check_forensic ${STAGEDIR}${PREFIX}/sbin

# maintainer only, check for new modules
modlist: extract
	@${AWK} '/: checking whether to enable mod_/ \
		{printf "%%%%%s%%%%libexec/apache24/%s.so\n", \
		toupper($$8), $$8}' ${WRKSRC}/configure.upstream \
		| ${TR} -d '"' \
		| ${SORT} -u \
		| ${GREP} -E -v '^%%MOD_(HTTP|ISAPI|LOG_CONFIG|PRIVILEGES|SO|UNIXD)%%'

.include <bsd.port.post.mk>
