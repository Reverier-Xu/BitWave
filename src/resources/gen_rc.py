#!/bin/env python3

import os


folders = ['assets', 'components', 'views']
qrc_content_start = \
'''<RCC>
    <qresource prefix="/">
'''
qrc_content_end = \
'''    </qresource>
</RCC>
'''

qrc_content = qrc_content_start

for folder in folders:
    for file in os.listdir(folder):
        qrc_content += f'        <file>{folder}/{file}</file>\n'

qrc_content += qrc_content_end
print(qrc_content)

with open('main.qrc', 'w') as out:
    out.write(qrc_content)

