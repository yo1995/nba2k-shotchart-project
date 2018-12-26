
import csv

import os
import re
import plotly.graph_objs as go
from plotly.offline import plot  # , init_notebook_mode
import base64
import sys

# import plotly
# plotly.tools.set_credentials_file(username='', api_key='')
# init_notebook_mode(connected=True)
''' import part ends. '''

debug = False
''' define part ends. '''

# nba-plan-line for sketch bg, nba-plan-draw for Thunders bg
# cwd = os.getcwd()  # deprecated

cwd = sys.path[0]
if debug:
    print(cwd)

nba_plan_path = cwd + '/Resources/nba-plan-line.png'
jordan_plot_path = cwd + '/Resources/jordan-plot.html'
with open(nba_plan_path, "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode()
# add the prefix that plot.ly will want when using the string as source
encoded_image = "data:image/png;base64," + encoded_string


print('0. initialize data.')

x3 = []
y3 = []
d3 = []

x2 = []
y2 = []
d2 = []


x1 = []
y1 = []
d1 = []

x0 = []
y0 = []
d0 = []

missed_shot_trace = []
made_shot_trace = []

# 初始化各项积累数值
MIN = 0
PTS = 0
FGM = 0
FGA = 0
PM3 = 0
PA3 = 0
FTM = 0
FTA = 0
REB = 0
AST = 0
STL = 0
BLK = 0
TOV = 0
PLM = 0

csv_file_list = []
''' initialization ends. '''


root_dir = cwd  # sys.path work under windows
# 读取当前目录下所有csv文件
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

if debug:
    print(csv_file_list)
''' read file path part ends. '''


print('1. start to parse data.')
for filename in csv_file_list:
    # to get folder years
    pattern = '\\\\([0-9]{4})\\\\'
    year = re.search(pattern, filename).group(0)
    year = year.replace("\\", "")

    with open(filename, 'r', encoding='UTF-8') as csv_file:
        reader = csv.reader(csv_file)
        old_rows = [row for row in reader]
    if debug:
        print(year)
        print(old_rows[-1][0])
    for row in old_rows:
        if len(row) < 3:
            # calculate sum-up data
            if row[0] == 'MIN':
                MIN = MIN + float(row[1])
                continue
            if row[0] == 'PTS':
                PTS = PTS + int(row[1])
                continue
            if row[0] == 'FGM':
                FGM = FGM + int(row[1])
                continue
            if row[0] == 'FGA':
                FGA = FGA + int(row[1])
                continue
            if row[0] == '3PM':
                PM3 = PM3 + int(row[1])
                continue
            if row[0] == '3PA':
                PA3 = PA3 + int(row[1])
                continue
            if row[0] == 'FTM':
                FTM = FTM + int(row[1])
                continue
            if row[0] == 'FTA':
                FTA = FTA + int(row[1])
                continue
            if row[0] == 'REB':
                REB = REB + int(row[1])
                continue
            if row[0] == 'AST':
                AST = AST + int(row[1])
                continue
            if row[0] == 'STL':
                STL = STL + int(row[1])
                continue
            if row[0] == 'BLK':
                BLK = BLK + int(row[1])
                continue
            if row[0] == 'TOV':
                TOV = TOV + int(row[1])
                continue
            if row[0] == 'PLM':
                PLM = PLM + int(row[1])
                continue
            continue
        # missed shots
        if row[1] == '0':
            # mid-court ring to rim = 12.72 meters
            if float(row[4]) < 1272 and float(row[2]) < 0:  # flip court each quarter, but
                x0.append('%.2f' % -float(row[2]))  # to prettify the output
                y0.append('%.2f' % -float(row[3]))
            else:
                x0.append('%.2f' % float(row[2]))  # to prettify the output
                y0.append('%.2f' % float(row[3]))
            d0.append('Missed<br>' + 'Distance: %.2f' % (float(row[4]) / 100) + 'm <br>' + year + '-' + str(int(year) + 1) + ' ' + old_rows[-1][0])

        # made shots
        if row[1] == '1':
            # mid-court ring to rim = 12.72 meters
            if int(row[5]) > 2:  # 3-pointers
                if float(row[4]) < 1272 and float(row[2]) < 0:  # flip court each quarter, but
                    x3.append('%.2f' % -float(row[2]))  # to prettify the output
                    y3.append('%.2f' % -float(row[3]))
                else:
                    x3.append('%.2f' % float(row[2]))  # to prettify the output
                    y3.append('%.2f' % float(row[3]))
                d3.append('3PT<br>' + 'Distance: %.2f' % (float(row[4]) / 100) + 'm <br>' + year + '-' + str(int(year) + 1) + ' ' + old_rows[-1][0])
            if 1 < int(row[5]) < 3:  # 2-pointers
                if float(row[4]) < 1272 and float(
                        row[2]) < 0:  # flip court each quarter, but
                    x2.append('%.2f' % -float(row[2]))  # to prettify the output
                    y2.append('%.2f' % -float(row[3]))
                else:
                    x2.append('%.2f' % float(row[2]))  # to prettify the output
                    y2.append('%.2f' % float(row[3]))
                if float(row[4]) < 1:  # dunk
                    d2.append('Dunk<br>' + 'Distance: %.2f' % (float(
                        row[4]) / 100) + 'm <br>' + year + '-' + str(
                        int(year) + 1) + ' ' +
                              old_rows[-1][0])
                else:
                    d2.append('2PT<br>' + 'Distance: %.2f' % (float(
                            row[4]) / 100) + 'm <br>' + year + '-' + str(
                            int(year) + 1) + ' ' +
                        old_rows[-1][0])

            if int(row[5]) < 2:  # FTs
                if float(row[4]) < 1272 and float(
                        row[2]) < 0:  # flip court each quarter, but
                    x1.append('%.2f' % -float(row[2]))  # to prettify the output
                    y1.append('%.2f' % -float(row[3]))

                else:
                    x1.append('%.2f' % float(row[2]))  # to prettify the output
                    y1.append('%.2f' % float(row[3]))
                d1.append('FT<br>' + 'Distance: %.2f' % (float(row[4]) / 100) + 'm <br>' + year + '-' + str(int(year) + 1) + ' ' + old_rows[-1][0])


print('2. make the coordinates!')
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

made_shot_trace1 = go.Scatter(
    x=x1,
    y=y1,
    hovertext=d1,
    hoverinfo='text',
    mode='markers',
    name='Made Free Throw',
    marker=dict(
        size=8,
        color='rgba(255, 255, 0, .8)',
        line=dict(
            width=1,
            color='rgb(0, 0, 0, 1)'
        )
    )
)

made_shot_trace2 = go.Scatter(
    x=x2,
    y=y2,
    hovertext=d2,
    hoverinfo='text',
    mode='markers',
    name='Made 2PTs Shot',
    marker=dict(
        size=8,
        color='rgba(0, 150, 100, .8)',
        line=dict(
            width=1,
            color='rgb(0, 0, 0, 1)'
        )
    )
)

made_shot_trace3 = go.Scatter(
    x=x3,
    y=y3,
    hovertext=d3,
    hoverinfo='text',
    mode='markers',
    name='Made 3PTs Shot',
    marker=dict(
        size=8,
        color='rgba(0, 255, 0, 1)',
        line=dict(
            width=1,
            color='rgb(0, 0, 0, 1)'
        )
    )
)

print('3. plot the figure!')
data = [missed_shot_trace, made_shot_trace2, made_shot_trace3, made_shot_trace1]
# sum_up = ['MIN', ('%.2f' % (MIN / 60)), 'PTS', PTS, 'FGM', FGM, 'FGA', FGA, '3PM', PM3, '3PA', PA3, 'FTM', FTM, 'FTA', FTA, 'REB', REB, 'AST', AST, 'STL', STL, 'BLK', BLK, 'TOV', TOV, 'PLM', PLM]

sum_text = ''
sum_text_l1 = ['MIN', 'PTS', 'FGM', 'FGA', '3PM', '3PA', 'FTM', 'FTA', 'REB', 'AST', 'STL', 'BLK', 'TOV', 'PLM']
sum_text_l2 = [('%.2f' % (MIN / 60)), PTS, FGM, FGA, PM3, PA3, FTM, FTA, REB, AST, STL, BLK, TOV, PLM]
for i in range(len(sum_text_l1)):
    sum_text += '|<b>' + sum_text_l1[i] + '</b>'
    sum_text += '|' + str(sum_text_l2[i])


title_text = 'Shots by Michael Jordan in NBA2K11 MyPlayer mode <br>' + str(sum_text)
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
    title=title_text,
    hovermode='closest',
    showlegend=True,
    height=873,
    width=1602
    # scene=dict(aspectmode="data")
)

fig = go.Figure(data=data, layout=layout)
plot(fig, filename=jordan_plot_path)
