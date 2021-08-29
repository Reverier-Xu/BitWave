/*
 * sqlite_engine.cpp
 *
 * Summary: wrapper of sqlite.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 *
 * Created: 2021-07-23
 * Last Modified: 2021-08-11
 *
 */

#include "sqlite_engine.h"

SQLiteEngine *SQLiteEngine::mInstance = nullptr;

SQLiteEngine::SQLiteEngine(QObject *parent) { }

SQLiteEngine *SQLiteEngine::instance(QObject *parent) {
    if (mInstance == nullptr)
        mInstance = new SQLiteEngine(parent);
    return mInstance;
}
