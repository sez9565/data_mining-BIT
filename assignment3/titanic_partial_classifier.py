import pandas as pd
from sklearn import tree
from sklearn.tree import DecisionTreeClassifier


def read_csv(path, na_values=None):
    """
    读取csv数据集
    """
    return pd.read_csv(path, na_values=na_values, low_memory=False)


titanic_data = read_csv("train.csv")
tree_data_cols_all = ["Age", "SibSp", "Parch", "Fare", "Pclass"]
tree_data_cols = ["Age", "SibSp", "Parch", "Fare"]
tree_target_cols = ["Pclass"]

tree_data_to_fit = titanic_data[tree_data_cols_all]
tree_data_to_fit = tree_data_to_fit.dropna()  # 直接去掉无年龄的行
tree_data = tree_data_to_fit[tree_data_cols]
tree_target = tree_data_to_fit[tree_target_cols]
tree_clf: DecisionTreeClassifier = tree.DecisionTreeClassifier(max_depth=5)
tree_clf = tree_clf.fit(tree_data.as_matrix(), tree_target.as_matrix())
with open("tree.dot", 'w') as f:
    f = tree.export_graphviz(tree_clf, out_file=f, feature_names=tree_data_cols, class_names=["1", "2", "3"])
