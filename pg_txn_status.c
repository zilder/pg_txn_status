#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/hsearch.h"
#include "utils/memutils.h"

#include "pg_txn_status.h"

PG_MODULE_MAGIC;

#define STATUS_NUM 6
#define UNKNOWN_STATUS "Unknown status: '%s'"

const char *int_to_text_map[] = {
	"begin",
	"prepare",
	"commit",
	"rollback",
	"complete",
	"incomplete"
};

PG_FUNCTION_INFO_V1(txn_status_out);
PG_FUNCTION_INFO_V1(txn_status_in);
PG_FUNCTION_INFO_V1(txn_status_eq);

/*
 * Output function
 */
Datum
txn_status_out(PG_FUNCTION_ARGS)
{
	char	status = PG_GETARG_CHAR(0);

	Assert(status >= 1 && status <= STATUS_NUM);
	PG_RETURN_CSTRING(pstrdup(int_to_text_map[status - 1]));
}

/*
 * Input function
 */
Datum
txn_status_in(PG_FUNCTION_ARGS)
{
	char   *statusStr = PG_GETARG_CSTRING(0);
	int		len = strlen(statusStr);
	struct status_code *sc;

	if (len == 0)
		elog(ERROR, "Empty status name");

	/* Use gperf generated hashing to obtain status code */
	sc = get_status_code(statusStr, len);
	if (!sc)
		elog(ERROR, UNKNOWN_STATUS, statusStr);

	PG_RETURN_CHAR(sc->code);
}

/*
 * Equality operator function
 */
Datum
txn_status_eq(PG_FUNCTION_ARGS)
{
	char	arg1 = PG_GETARG_CHAR(0);
	char	arg2 = PG_GETARG_CHAR(1);

	PG_RETURN_BOOL(arg1 == arg2);
}
