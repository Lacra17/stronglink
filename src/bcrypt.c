#include "../deps/crypt_blowfish-1.0.4/ow-crypt.h"
#include "bcrypt.h"
#include "async.h"

int passcmp(volatile strarg_t const a, volatile strarg_t const b) {
	int r = 0;
	for(off_t i = 0; ; ++i) {
		if(a[i] != b[i]) r = -1;
		if(!a[i] || !b[i]) break;
	}
	return r;
}
bool_t checkpass(strarg_t const pass, strarg_t const hash) {
	int size = 0;
	void *data = NULL;
	strarg_t attempt = crypt_ra(pass, hash, &data, &size);
	bool_t const success = (attempt && 0 == passcmp(attempt, hash));
	FREE(&data); attempt = NULL;
	return success;
}
str_t *hashpass(strarg_t const pass) {
	int size = 0;
	void *data = NULL;
	char input[GENSALT_INPUT_SIZE];
	async_random((byte_t *)input, GENSALT_INPUT_SIZE);
	char *salt = crypt_gensalt_ra("$2a$", 8, input, GENSALT_INPUT_SIZE); // TODO: Use `$2y$` now? bcrypt library needs updating.
	str_t *hash = strdup(crypt_ra(pass, salt, &data, &size));
	FREE(&salt);
	FREE(&data);
	return hash;
}
