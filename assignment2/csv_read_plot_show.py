import math
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import statsmodels.api as sm
from pandas import DataFrame, Series


def set_display():
    """
    显示设置
    """
    pd.set_option('display.max_columns', 2000)
    pd.set_option('display.max_rows', 2000)
    pd.set_option('display.width', 2000)


def read_csv(path, na_values=None):
    """
    读取csv数据集
    """
    return pd.read_csv(path, na_values=na_values, low_memory=False)


def count(data_frame, columns, dropna=False, format_width=30, file=sys.stdout):
    """
    标称属性，给出每个可能取值的频数
    """
    format_text = '{{:<{0}}}{{:<{0}}}'.format(format_width)
    for col in columns:
        print('标称属性 <{}> 频数统计'.format(col), file=file)
        print(format_text.format('value', 'count'), file=file)
        print('~ ' * format_width, file=file)

        counts = pd.value_counts(data_frame[col].values, dropna=dropna)
        for i, index in enumerate(counts.index):
            if pd.isnull(index):  # NaN?
                print(format_text.format('-NaN-', counts.values[i]), file=file)
            else:
                print(format_text.format(index, counts[index]), file=file)
        print('~~' * format_width, file=file)
        print('\n', file=file)


# 绘图配置
row_size = 2
col_size = 3
cell_size = row_size * col_size


def histogram(data_frame, columns):
    """
    直方图
    """
    for i, col in enumerate(columns):
        if i % cell_size == 0:
            fig = plt.figure()
        ax = fig.add_subplot(col_size, row_size, (i % cell_size) + 1)
        data_frame[col].hist(ax=ax, grid=False, figsize=(15, 15), bins=50)
        plt.title(col)
        if (i + 1) % cell_size == 0 or i + 1 == len(columns):
            plt.subplots_adjust(wspace=0.3, hspace=0.3)
            plt.show()


def qqplot(data_frame, columns):
    """
    qq图
    """
    for i, col in enumerate(columns):
        if i % cell_size == 0:
            fig = plt.figure(figsize=(15, 15))
        ax = fig.add_subplot(col_size, row_size, (i % cell_size) + 1)
        sm.qqplot(data_frame[col], ax=ax)
        ax.set_title(col)
        if (i + 1) % cell_size == 0 or i + 1 == len(columns):
            plt.subplots_adjust(wspace=0.3, hspace=0.3)
            plt.show()


def compare(df1, df2, columns, bins=50):
    """
    直方图比较
    """
    for col in columns:
        mean1 = df1[col].mean()
        mean2 = df2[col].mean()

        fig = plt.figure()
        ax1 = fig.add_subplot(121)
        df1[col].hist(ax=ax1, grid=False, figsize=(15, 5), bins=bins)
        ax1.axvline(mean1, color='r')
        plt.title('origin\n{}\nmean={}'.format(col, str(mean1)))
        ax2 = fig.add_subplot(122)
        df2[col].hist(ax=ax2, grid=False, figsize=(15, 5), bins=bins)
        ax2.axvline(mean2, color='b')
        plt.title('filled\n{}\nmean={}'.format(col, str(mean2)))
        plt.subplots_adjust(wspace=0.3, hspace=10)
        plt.show()


def boxplot(dataFrame, columns):
    """
    盒图
    """
    for i, col in enumerate(columns):
        if i % cell_size == 0:
            fig = plt.figure()
        ax = fig.add_subplot(col_size, row_size, (i % cell_size) + 1)
        dataFrame[col].plot.box(ax=ax, figsize=(15, 15))
        if (i + 1) % cell_size == 0 or i + 1 == len(columns):
            plt.subplots_adjust(wspace=0.3, hspace=0.3)
            plt.show()


def describe(data_frame, columns, file=sys.stdout):
    """
    数值属性，给出最大、最小、均值、中位数、四分位数及缺失值的个数
    """
    desc = data_frame[columns].describe()
    statistic = DataFrame()
    statistic['max'] = desc.loc['max']
    statistic['min'] = desc.loc['min']
    statistic['mean'] = desc.loc['mean']
    statistic['50%'] = desc.loc['50%']
    statistic['25%'] = desc.loc['25%']
    statistic['75%'] = desc.loc['75%']
    statistic['NaN'] = data_frame[columns].isnull().sum()
    print(statistic, file=file)


if __name__ == '__main__':
    pass
