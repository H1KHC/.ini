#include "Error.h"

static INI_ERR error;

INI_ERR iniGetError() {
    INI_ERR e = error;
    error = INI_ERR_NO_ERROR;
    return e;
}

void setError(INI_ERR err) {
    error = err;
}