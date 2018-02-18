#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/hsearch.h"
#include "utils/memutils.h"

PG_MODULE_MAGIC;

#define STATUS_NUM 6
#define UNKNOWN_STATUS "Unknown status: '%s'"

#define match_tail(str1, str2) \
do { \
	if (strcmp((str1) + 1, (str2) + 1) != 0) \
		elog(ERROR, UNKNOWN_STATUS, str1); \
} while(0)

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
	PG_RETURN_CSTRING(int_to_text_map[status - 1]);
}

/*
 * Input function
 */
Datum
txn_status_in(PG_FUNCTION_ARGS)
{
	char   *statusStr = PG_GETARG_CSTRING(0);
	int		len = strlen(statusStr);
	char	result;

	if (len == 0)
		elog(ERROR, "Empty status name");

	/*
	 * Use fast filter by the leading letter first, then compare the rest
	 * of the status literal
	 */
	switch (statusStr[0])
	{
		case 'b': match_tail(statusStr, "begin"); result = 1; break;
		case 'p': match_tail(statusStr, "prepare"); result = 2; break;
		case 'r': match_tail(statusStr, "rollback"); result = 4; break;
		case 'i': match_tail(statusStr, "incomplete"); result = 6; break;
		case 'c':
			/* Two status names start with 'c' */
			if (strcmp(statusStr, "commit") == 0)
			{
				result = 3;
				break;
			}
			if (strcmp(statusStr, "complete") == 0)
			{
				result = 5;
				break;
			}
		default:
			elog(ERROR, UNKNOWN_STATUS, statusStr);
	}

	PG_RETURN_CHAR(result);
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
