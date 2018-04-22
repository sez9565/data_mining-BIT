from typing import Any, Union

from pandas import DataFrame
from pandas.io.parsers import TextFileReader

from csv_read_plot_show import *
from apriori import *

dataset_path = './Building_Permits.csv'
df = read_csv(dataset_path, na_values='None')
# 标称属性
name_category = ['Permit Type', 'Block', 'Lot', 'Street Number', 'Street Number Suffix', 'Street Name', 'Street Suffix',
                 'Current Status', 'Structural Notification', 'Voluntary Soft-Story Retrofit', 'Fire Only Permit',
                 'Existing Use', 'Proposed Use', 'Plansets', 'TIDF Compliance', 'Existing Construction Type',
                 'Proposed Construction Type', 'Site Permit', 'Supervisor District',
                 'Neighborhoods - Analysis Boundaries']
# 数值属性
name_value = ['Number of Existing Stories', 'Number of Proposed Stories', 'Estimated Cost', 'Revised Cost',
              'Existing Units', 'Proposed Units']

set_display()
name_cat_cnt_out_file = open("name_cat_cnt_out.txt", "w", encoding="UTF-8")
count(df, name_category, file=name_cat_cnt_out_file)
name_cat_out_file = open("name_cat_out.txt", "w")#, encoding="UTF-8")
describe(df, name_value)#, file=describe_out_file)

# 使用 N 填充空值
cols = ['Structural Notification', 'Voluntary Soft-Story Retrofit', 'Fire Only Permit', 'TIDF Compliance']
df_fillna = df[cols].fillna('N')
df[cols] = df_fillna[cols]
#print('                  旧数据\n')
#count(df, cols)

#print('\n\n                  新数据\n')
#count(df_fillna, cols)

data_set = df[name_category].values
# print(data_set)
for t in data_set:
    for item in t:
        print(item, end=' ', file=name_cat_out_file)
    print(file=name_cat_out_file)

# 注释了这段apriori的
# L, support_data = generate_L(data_set, k=3, min_support=0.02)
# big_rules_list = generate_big_rules(L, support_data, min_conf=0.7)
# for Lk in L:
#    print("="*50)
#    print("frequent " + str(len(list(Lk)[0])) + "-itemsets\t\tsupport")
#    print("="*50)
#    for freq_set in Lk:
#         print(freq_set, support_data[freq_set])
# print()
# print("Big Rules")
# for item in big_rules_list:
#     print(item[0], "=>", item[1], "conf: ", item[2])


