import csv

import os
import re
import plotly.graph_objs as go
from plotly.offline import init_notebook_mode, plot
import base64
import time
import sys

# import plotly
# plotly.tools.set_credentials_file(username='', api_key='')
# init_notebook_mode(connected=True)


# nba-plan-line for sketch bg, nba-plan-draw for Thunders bg
# cwd = os.getcwd()
# print(cwd)
cwd = sys.path[0]

nba_plan_path = cwd + '/Resources/nba-plan-line.png'
jordan_plot_path = cwd + '/Resources/jordan-plot.html'
with open(nba_plan_path, "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode()
# add the prefix that plotly will want when using the string as source
encoded_image = "data:image/png;base64," + encoded_string

x1 = []
y1 = []
d1 = []

x0 = []
y0 = []
d0 = []

# 读取当前目录下所有csv文件
csv_file_list = []
root_dir = cwd  # sys.path work under windows

'''
for filename in os.listdir(root_dir):
    pathname = os.path.join(root_dir, filename)
    if os.path.splitext(filename)[1] == '.csv':
        csv_file_list.append(pathname)
'''

# 递归所有csv文件
for root, dirs, files in os.walk(root_dir):  
    for file in files:  
        if os.path.splitext(file)[1] == '.csv':  
            csv_file_list.append(os.path.join(root, file))


print(csv_file_list)

missed_shot_trace =[]
made_shot_trace = []

print('1. start to parse data.')
for filename in csv_file_list:
    # to get folder years
    pattern = '\\\\([0-9]{4})\\\\'
    year = re.search(pattern, filename).group(0)
    year = year.replace("\\", "")

    with open(filename, 'r', encoding='UTF-8') as csv_file:
        reader = csv.reader(csv_file)
        old_rows = [row for row in reader]
    # print(year)
    # print(old_rows[-1][0])
    for row in old_rows:
        if len(row) < 3:
            continue
        if row[1] == '0':
            # mid-court ring to rim = 12.72 meters
            if float(row[4]) < 1272 and float(row[2]) < 0:  # flip court each quarter, but
                x0.append('%.2f' % -float(row[2]))  # to prettify the output
                y0.append('%.2f' % -float(row[3]))
                d0.append(
                    'Distance: %.2f' % (float(row[4]) / 100) + 'm <br>' + year + '-' + str(int(year) + 1) + ' ' +
                    old_rows[-1][0])
            else:
                x0.append('%.2f' % float(row[2]))  # to prettify the output
                y0.append('%.2f' % float(row[3]))
                d0.append(
                    'Distance: %.2f' % (float(row[4]) / 100) + 'm <br>' + year + '-' + str(int(year) + 1) + ' ' +
                    old_rows[-1][0])

    for row in old_rows:
        if len(row) < 3:
            continue
        if row[1] == '1':
            # mid-court ring to rim = 12.72 meters
            if float(row[4]) < 1272 and float(row[2]) < 0:  # flip court each quarter, but
                x1.append('%.2f' % -float(row[2]))  # to prettify the output
                y1.append('%.2f' % -float(row[3]))
                d1.append(
                    'Distance: %.2f' % (float(row[4]) / 100) + 'm <br>' + year + '-' + str(int(year) + 1) + ' ' + old_rows[-1][0])
            else:
                x1.append('%.2f' % float(row[2]))  # to prettify the output
                y1.append('%.2f' % float(row[3]))
                d1.append(
                    'Distance: %.2f' % (float(row[4]) / 100) + 'm <br>' + year + '-' + str(int(year) + 1) + ' ' +
                    old_rows[-1][0])


print('2. make the coords!')
missed_shot_trace = go.Scatter(
    x=x0,
    y=y0,
    hovertext=d0,
    hoverinfo='text',
    mode='markers',
    name='Missed Shot',
    marker=dict(
        symbol='x',
        size=8,
        color='rgba(255, 0, 0, .8)',
        line=dict(
            width=1,
            color='rgb(0, 0, 0, 1)'
        )
    )
)

made_shot_trace = go.Scatter(
    x=x1,
    y=y1,
    hovertext=d1,
    hoverinfo='text',
    mode='markers',
    name='Made Shot',
    marker=dict(
        size=8,
        color='rgba(0, 200, 100, .8)',
        line=dict(
            width=1,
            color='rgb(0, 0, 0, 1)'
        )
    )
)


print('3. plot the figure!')
data = [missed_shot_trace, made_shot_trace]
layout = go.Layout(
    images=[dict(
          source=encoded_image,
          xref="paper",
          yref="paper",
          xanchor='left',
          yanchor='bottom',
          sizex=1,
          sizey=1,
          sizing="stretch",
          opacity=0.5,
          layer="below")],
    xaxis=dict(
        visible=False,
        range=[-1500, 1500],
        fixedrange=True,
                ),
    yaxis=dict(
        visible=False,
        range=[-800, 800],
        fixedrange=True,
                ),
    title='Shots by Michael Jordan in NBA2K11 MyPlayer mode',
    hovermode='closest',
    showlegend=True,
    height=873,
    width=1602
    # scene=dict(aspectmode="data")
)

fig = go.Figure(data=data, layout=layout)
plot(fig, filename=jordan_plot_path)
