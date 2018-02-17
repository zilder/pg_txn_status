# contrib/intarray/Makefile

MODULE_big = pg_txn_status
OBJS = pg_txn_status.o  $(WIN32RES)

EXTENSION = pg_txn_status
DATA = pg_txn_status--0.1.sql
PGFILEDESC = "1 byte transaction status type"

REGRESS = test_txn_status

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/pg_txn_status
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
