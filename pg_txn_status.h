#ifndef PG_TXN_STATUS_H
#define PG_TXN_STATUS_H

struct status_code {
	char   *status;
	char	code;
};

struct status_code *get_status_code(const char *str, unsigned int len);

#endif /* PG_TXN_STATUS_H */