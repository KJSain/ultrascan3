include( ../../gui.pri )

QT           += svg
QT           += sql
TARGET        = us_config
TRANSLATIONS += $${TARGET}_de_DE.ts

HEADERS      += us_config.h   \
                us_admin.h    \
                us_color.h    \
                us_database.h \
                us_advanced.h \
                us_font.h     \
                us_xpnhost.h  \
                us_xpnhost_db.h \
                us_newxpnhost_db.h

SOURCES      += us_config.cpp   \
                us_admin.cpp    \
                us_color.cpp    \
                us_database.cpp \
                us_advanced.cpp \
                us_font.cpp     \
                us_xpnhost.cpp  \
                us_xpnhost_db.cpp \
                us_newxpnhost_db.cpp 
                
