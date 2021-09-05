#!/bin/env python3

import os

folder = 'assets/'

for file in os.listdir(folder):
    new_file = file.replace('ic_fluent_', '').replace('_', '-').replace('-regular', '')
    os.rename(f'{folder}/{file}', f'{folder}/{new_file}')


