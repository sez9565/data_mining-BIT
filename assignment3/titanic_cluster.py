from sklearn.cluster import KMeans
from sklearn.cluster import DBSCAN
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def read_csv(path, na_values=None):
    """
    读取csv数据集
    """
    return pd.read_csv(path, na_values=na_values, low_memory=False)


titanic_data = read_csv("train.csv")
cluster_data_cols = ["Age", "Fare"]
cluster_data = titanic_data[cluster_data_cols]
cluster_data = cluster_data.dropna()  # 直接去掉无年龄的行

fig1 = plt.figure(1)
sp1 = fig1.add_subplot(211)
kmeans_pred = KMeans(n_clusters=3, random_state=9).fit_predict(cluster_data.as_matrix())
sp1.scatter(cluster_data["Age"].values, cluster_data["Fare"].values, c=kmeans_pred)
# plt.show()


kmeans_pred = KMeans(n_clusters=4, random_state=9).fit_predict(cluster_data.as_matrix())
sp2 = fig1.add_subplot(212)
sp2.scatter(cluster_data["Age"].values, cluster_data["Fare"].values, c=kmeans_pred)
# plt.show()

fig2 = plt.figure(2)
dbscan_pred = DBSCAN(eps=1).fit_predict(cluster_data.as_matrix())
sp2_1 = fig2.add_subplot(131)
sp2_1.scatter(cluster_data["Age"].values, cluster_data["Fare"].values, c=dbscan_pred)

dbscan_pred = DBSCAN(eps=3).fit_predict(cluster_data.as_matrix())
sp2_2 = fig2.add_subplot(132)
sp2_2.scatter(cluster_data["Age"].values, cluster_data["Fare"].values, c=dbscan_pred)

dbscan_pred = DBSCAN(eps=5).fit_predict(cluster_data.as_matrix())
sp2_3 = fig2.add_subplot(133)
sp2_3.scatter(cluster_data["Age"].values, cluster_data["Fare"].values, c=dbscan_pred)

plt.show()

