# $FreeBSD: head/www/nextcloud/Makefile 452581 2017-10-21 09:20:39Z brnrd $

PORTNAME=	nextcloud
DISTVERSION=	13.0.2
CATEGORIES=	www
MASTER_SITES=	https://download.nextcloud.com/server/releases/
PKGNAMESUFFIX=	${PHP_PKGNAMESUFFIX}

MAINTAINER=	brnrd@FreeBSD.org
COMMENT=	Personal cloud which runs on your own server

LICENSE=	AGPLv3

USES=		cpe gettext-runtime php:flavors,web tar:bzip2 ssl
USE_PHP=	bz2 ctype curl dom fileinfo filter gd hash iconv json \
		mbstring pdo posix session simplexml xml xmlreader xmlwriter \
		xsl wddx zip zlib

NEXTCLOUD_USERNAME?=	${WWWOWN}
NEXTCLOUD_GROUPNAME?=	${WWWGRP}

WRKSRC=		${WRKDIR}/${PORTNAME}
NO_BUILD=	yes
NO_ARCH=	yes
SUB_FILES=	pkg-message
PLIST_SUB=	NEXTCLOUD_USERNAME=${NEXTCLOUD_USERNAME} \
		NEXTCLOUD_GROUPNAME=${NEXTCLOUD_GROUPNAME}

OPTIONS_DEFINE=	EXIF LDAP OPCACHE SMB SSL
OPTIONS_MULTI=	DB
OPTIONS_MULTI_DB=	MYSQL PGSQL SQLITE
OPTIONS_DEFAULT=	EXIF LDAP MYSQL OPCACHE SSL

DB_DESC=	Database backend(s)
EXIF_DESC=	Image rotation support
OPCACHE_DESC=	Enable (Nextcloud recommended) OpCache

EXIF_USE=	PHP=exif
LDAP_USE=	PHP=ldap
MYSQL_USES=	mysql
MYSQL_USE=	PHP=pdo_mysql
OPCACHE_USE=	PHP=opcache
PGSQL_USES=	pgsql
PGSQL_USE=	PHP=pdo_pgsql,pgsql
SMB_RUN_DEPENDS=	${PECL_PKGNAMEPREFIX}smbclient>=0.9.0:net/pecl-smbclient@${PHP_FLAVOR}
SQLITE_USE=	PHP=pdo_sqlite,sqlite3
SSL_USE=	PHP=openssl

post-patch:
	@${REINPLACE_CMD} -e 's|/var/www/${PORTNAME}|${WWWDIR}|' ${WRKSRC}/config/config.sample.php
	@${MV} ${WRKSRC}/apps ${WRKSRC}/apps-pkg

do-install:
	@${MKDIR} -m 0755 ${STAGEDIR}${WWWDIR}
	@cd ${WRKSRC} && ${COPYTREE_SHARE} . ${STAGEDIR}${WWWDIR}
	@${RM} ${STAGEDIR}${WWWDIR}/updater/updater.phar
	@${MKDIR} ${STAGEDIR}${WWWDIR}/data
	@${MKDIR} ${STAGEDIR}${WWWDIR}/apps
	@${MKDIR} ${STAGEDIR}${DATADIR}
	${INSTALL_DATA} ${FILESDIR}/fix-apps_paths.json ${STAGEDIR}${DATADIR}/fix-apps_paths.json

.include <bsd.port.mk>
