from csv_read_plot_show import *

dataset_path = './Building_Permits.csv'  # 替换为数据集所在路径或将其复制到当前工作目录
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
name_cat_cnt_out_file = open("name_cat_out.txt", "w", encoding="UTF-8")
count(df, name_category, file=name_cat_cnt_out_file)
describe_out_file = open("describe_out.txt", "w", encoding="UTF-8")
describe(df, name_value)#, file=describe_out_file)

# 直方图
histogram(df, name_value)

# q-q图
qqplot(df, name_value)

# 盒图
boxplot(df, name_value)

# 使用 N 填充空值
cols = ['Structural Notification', 'Voluntary Soft-Story Retrofit', 'Fire Only Permit', 'TIDF Compliance']
df_fillna = df[cols].fillna('N')

print('                  旧数据\n')
count(df, cols)

print('\n\n                  新数据\n')
count(df_fillna, cols)
