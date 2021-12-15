#include <stdio.h>
#include <string.h>

#include "map.h"
#include "sds.h"

static int get_last_item(map_int_t mp) {
    const char *key;
    map_iter_t iter = map_iter(&mp);

    do {
        key = map_next(&mp, &iter);
    } while (key);

    return key == NULL ? -1 : *map_get(&mp, key);
}

static void get_values(const char *rem_str, const int *value, map_int_t *mp, int *value_int, int *last_item) {
    value = map_get(mp, rem_str);
    (*value_int) = value ? *value : -1;
    (*last_item) = get_last_item((*mp));
}

static sds fractionToDecimal(int numr, int denr) {
    map_int_t mp;
    sds res = sdsempty();
    int value_int, last_item_int, rem = numr % denr;
    int *value;
    char rem_str[128];

    sprintf(rem_str, "%d", rem);
    map_init(&mp);

    get_values(rem_str, value, &mp, &value_int, &last_item_int);

    while (rem != 0 && value_int == last_item_int) {
        map_set(&mp, rem_str, strlen(res));

        rem = rem * 10;

        int res_part = rem / denr;
        char res_part_arr[128];

        sprintf(res_part_arr, "%d", res_part);

        res = sdscat(res, res_part_arr);

        rem = rem % denr;
        sprintf(rem_str, "%d", rem);

        get_values(rem_str, value, &mp, &value_int, &last_item_int);
    }

    if (!rem)
        return NULL;

    sdsrange(res, *map_get(&mp, rem_str), sdslen(res));

    map_deinit(&mp);

    return res;
}

int main() {
    sds str, val_str, res;
    sds *tokens;
    int n, d, count;
    double val;

    scanf("%d%d", &n, &d);

    str = fractionToDecimal(n, d);
    val = n / (double) d;
    val_str = sdscatprintf(sdsempty(), "%.10f\n", val);

    if (str) {
        tokens = sdssplitlen(val_str, sdslen(val_str), str, sdslen(str), &count);

        res = sdsempty();
        res = sdscat(res, tokens[0]);
        res = sdscat(res, "[");
        res = sdscat(res, str);
        res = sdscat(res, "]");

        printf("%s", res);

        sdsfreesplitres(tokens, count);
        sdsfree(res);
    } else {
        printf("%.10f", val);
    }

    sdsfree(val_str);
    sdsfree(str);

    return 0;
}

