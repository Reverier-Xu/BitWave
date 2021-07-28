//
// Created by Reverier-Xu on 2021/7/23.
//

#include "sqlite_engine.h"

SQLiteEngine* SQLiteEngine::mInstance = nullptr;

SQLiteEngine::SQLiteEngine(QObject* parent) {}

SQLiteEngine* SQLiteEngine::getInstance(QObject* parent) {
    if (mInstance == nullptr) mInstance = new SQLiteEngine(parent);
    return mInstance;
}
