# Created by: John W. O'Brien <john@saltant.com>
# $FreeBSD$

PORTNAME=	Flask-Kerberos
PORTVERSION=	1.0.4
CATEGORIES=	security www python
MASTER_SITES=	CHEESESHOP
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	john@saltant.com
COMMENT=	GSSAPI Negotiate HTTP authentication for Flask routes

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}Flask>0:www/py-flask@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}kerberos>1.1.1:security/py-kerberos@${FLAVOR}

USES=		python
USE_PYTHON=	autoplist distutils

NO_ARCH=	yes

.include <bsd.port.mk>
