# pg_txn_status

This extension provides 1 byte `txn_status` type for transaction status
representation.

## Installation

To build and install this extension run:

```
make install USE_PGXS=1
```

or if postgres binaries are not on PATH:

```
make install USE_PGXS=1 PG_CONFIG=/path/to/pg_config
```

## Using

`txn_status` type works similar to enumerated types. It provides six builtin
values:

* `begin`
* `prepare`
* `commit`
* `rollback`
* `complete`
* `incomplete`

Example:

```
CREATE TABLE test (
	id serial,
	status txn_status
);

INSERT INTO test (status) VALUES ('prepare'), ('commit'), ('rollback');

SELECT * FROM test WHERE status = 'commit';
 id | status
----+--------
  2 | commit
(1 row)
```

Only equality operator is supported at the moment.