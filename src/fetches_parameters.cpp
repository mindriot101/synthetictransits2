#include "fetches_parameters.h"
#include <cstdlib>
#include <sqlite3.h>

using namespace std;

FetchesParameters::FetchesParameters(const string &filename)
    : filename(filename) {
        sqlite3_open(filename.c_str(), &ppDb);
    }

FetchesParameters::~FetchesParameters() {
    if (ppDb != NULL) {
        sqlite3_close(ppDb);
    }
}

Model FetchesParameters::model_from_statement(sqlite3_stmt *stmt) {
    Model model;
    int col = 0;
    model.id = sqlite3_column_int(stmt, col++);

    const unsigned char *model_name_cstr = sqlite3_column_text(stmt, col++);
    string model_name(reinterpret_cast<const char*>(model_name_cstr));
    model.name = model_name;

    model.submodel_id = sqlite3_column_int(stmt, col++);
    model.period = sqlite3_column_double(stmt, col++);
    model.epoch = sqlite3_column_double(stmt, col++);
    model.a =  sqlite3_column_double(stmt, col++);
    model.i = sqlite3_column_double(stmt, col++);
    model.rs = sqlite3_column_double(stmt, col++);
    model.rp = sqlite3_column_double(stmt, col++);
    model.mstar = sqlite3_column_double(stmt, col++);
    model.c1 = sqlite3_column_double(stmt, col++);
    model.c2 = sqlite3_column_double(stmt, col++);
    model.c3 = sqlite3_column_double(stmt, col++);
    model.c4 = sqlite3_column_double(stmt, col++);
    model.teff = sqlite3_column_double(stmt, col++);

    return model;
}

// TODO: why are some names NULL?
vector<Model> FetchesParameters::fetch_models() {
    const string sql = "select id, name, submodel_id, period, epoch, a, "
        "i, rs, rp, mstar, c1, c2, c3, c4, teff "
        "from addmodels "
        "where name is not null "
        "and period is not null "
        "and epoch is not null "
        "and a is not null "
        "and i is not null "
        "and rs is not null "
        "and rp is not null "
        "and mstar is not null "
        "and c1 is not null "
        "and c2 is not null "
        "and c3 is not null "
        "and c4 is not null "
        "and teff is not null "
        "order by name asc"
        ;
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(ppDb, sql.c_str(), sql.size(), &stmt, NULL);
    vector<Model> models;
    int step_state = 0;
    while (step_state != SQLITE_DONE) {
        switch (step_state) {
            case SQLITE_ROW:
                models.push_back(model_from_statement(stmt));
                break;
            case SQLITE_ERROR:
                fprintf(stderr, "Error\n");
                exit(1);
        }
        step_state = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);

    return models;
}