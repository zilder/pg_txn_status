MODULE_big = pg_txn_status
OBJS = pg_txn_status.o  $(WIN32RES)

EXTENSION = pg_txn_status
DATA = pg_txn_status--0.1.sql
PGFILEDESC = "1 byte transaction status type"

REGRESS = test_txn_status

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)