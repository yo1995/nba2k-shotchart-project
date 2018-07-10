import csv

import os
import plotly.graph_objs as go
from plotly.offline import init_notebook_mode, plot
import base64


# import plotly
# plotly.tools.set_credentials_file(username='', api_key='')

# init_notebook_mode(connected=True)


# nba-plan-line for sketch bg, nba-plan-draw for Thunders bg
with open("./Resources/nba-plan-line.png", "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode()
# add the prefix that plotly will want when using the string as source
encoded_image = "data:image/png;base64," + encoded_string

x = []
y = []
d = []

# 读取当前目录下所有csv文件
csv_file_list = []
root_dir = os.getcwd()

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
'''

print(csv_file_list)

missed_shot_trace =[]
made_shot_trace = []

print('1. start to parse data.')
for filename in csv_file_list:
    with open(filename, 'r', encoding='UTF-8') as csv_file:
        reader = csv.reader(csv_file)
        old_rows = [row for row in reader]

    for row in old_rows:
        if len(row) < 3:
            continue
        if row[1] == '0':
            x.append('%.2f' % float(row[2]))
            y.append('%.2f' % float(row[3]))
            d.append('%.2f' % float(row[4]))

    missed_shot_trace = go.Scatter(
        x=x,
        y=y,
        hovertext=d,
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

    x = []
    y = []
    d = []
    for row in old_rows:
        if len(row) < 3:
            continue
        if row[1] == '1':
            x.append('%.2f' % float(row[2]))  # to prettify the output
            y.append('%.2f' % float(row[3]))
            d.append('%.2f' % float(row[4]))

    made_shot_trace = go.Scatter(
        x=x,
        y=y,
        hovertext=d,
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
    showlegend=True,
    height=873,
    width=1602
    # scene=dict(aspectmode="data")
)

fig = go.Figure(data=data, layout=layout)
plot(fig, filename='./docs/jordan-plot.html')
