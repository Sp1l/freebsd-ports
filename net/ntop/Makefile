# Created by: Bill Fumerola <billf@chc-chimes.com>
# $FreeBSD$

PORTNAME=	ntop
PORTVERSION=	5.0.1
PORTREVISION=	13
CATEGORIES=	net
MASTER_SITES=	SF/${PORTNAME}/${PORTNAME}/Stable

MAINTAINER=	amutu@amutu.com
COMMENT=	Network monitoring tool with command line and web interfaces

LICENSE=	GPLv3
LICENSE_FILE=	${WRKSRC}/COPYING

LIB_DEPENDS=	libgdbm.so:databases/gdbm \
		libevent.so:devel/libevent
BUILD_DEPENDS=	rrdtool12>=1.2:databases/rrdtool12 \
		dot:graphics/graphviz \
		geoiplookup:net/GeoIP
RUN_DEPENDS:=	${BUILD_DEPENDS} \
		urwfonts>0:x11-fonts/urwfonts

BROKEN_powerpc64=	fails to link: libntop.so: undefined reference to 'bswap_32'

DBDIR?=		/var/db

USES=		autoreconf:build cpe gmake libtool python shebangfix ssl
SHEBANG_FILES=	python/sankey.py
USE_GNOME=	libxml2
GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes
PLIST_SUB=	DBDIR=${DBDIR} \
		PORTVERSION=${PORTVERSION}
CONFIGURE_ARGS=	--localstatedir=${DBDIR} \
		--with-ossl-root=${OPENSSLBASE} \
		--with-gdbm-root=${LOCALBASE} \
		--with-zlib-root=/usr

CPPFLAGS+=	-I${LOCALBASE}/include
LIBS+=		-L${LOCALBASE}/lib
PKGDEINSTALL=	${WRKDIR}/pkg-deinstall
SUB_FILES=	pkg-deinstall
SUB_LIST=	DBDIR=${DBDIR}
USE_RC_SUBR=	ntop

OPTIONS_DEFINE=	PCAP_PORT XMLDUMP JUMBO_FRAMES MAKO

PCAP_PORT_DESC=		Use libpcap from ports
XMLDUMP_DESC=		Enable XML Dump support
JUMBO_FRAMES_DESC=	Jumbo Frames (experimental)
MAKO_DESC=		Install py-mako for Host Map, Region View

PCAP_PORT_LIB_DEPENDS=	libpcap.so.1:net/libpcap
PCAP_PORT_CONFIGURE_ON=	--with-pcap-root=${LOCALBASE}
PCAP_PORT_CONFIGURE_OFF=--with-pcap-root=/usr

XMLDUMP_LIB_DEPENDS=	libgdome.so:textproc/gdome2
XMLDUMP_CPPFLAGS=	-I${LOCALBASE}/include/libxml2 \
			-I${LOCALBASE}/include/libxml2/libxml \
			-I${LOCALBASE}/include/libgdome \
			-I${LOCALBASE}/include/glib-2.0

# should set your mtu to 9000
# see http://www.cyberciti.biz/faq/freebsd-jumbo-frames-networking-configration/
JUMBO_FRAMES_CONFIGURE_ON=	--enable-jumbo-frames

MAKO_RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}mako>0:textproc/py-mako@${PY_FLAVOR}

pre-configure:
	@(cd ${WRKSRC} && ./autogen.sh --noconfig)

pre-install:
	@cd ${WRKSRC} && ${MAKE_CMD} dnetter

post-install:
	@${STRIP_CMD} ${STAGEDIR}${PREFIX}/lib/*.so \
		${STAGEDIR}${PREFIX}/lib/ntop/plugins/*.so

.include <bsd.port.mk>
