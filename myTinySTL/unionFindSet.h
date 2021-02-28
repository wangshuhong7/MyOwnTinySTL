#pragma once
#ifndef __UNION_FIND_SET_
#define __UNION_FIND_SET_
#include<cstring>
namespace myTinySTL {
	template<size_t n>
	class unionFindSet {
	public:
		unionFindSet();
		int find(int index);
		void merge(int index1, int index2);
		void clear();
	private:
		int parent[n];//parent[i] = -n 表示节点i是根节点且以i为根的树中共有n个节点

	};
	template<size_t n>
	unionFindSet<n>::unionFindSet() { clear(); }

	template<size_t n>
	int unionFindSet<n>::find(int index) {
		auto root = index;
		for (; parent[root] >= 0; root = parent[root]) {}
		while (root != index) {//路径压缩
			auto t = parent[index];
			parent[index] = root;//全部子点都直接链接到根
			index = t;
		}
		return root;
	}
	template<size_t n>
	void unionFindSet<n>::merge(int index1, int index2) {
		auto root1 = Find(index1), root2 = Find(index2);
		auto total_nodes = parent[root1] + parent[root2];//total nodes
		if (parent[root1] > parent[root2]) {//加权合并（由于已经find所以都是负数）
			parent[root1] = root2;
			parent[root2] = total_nodes;
		}
		else {
			parent[root2] = root1;
			parent[root1] = total_nodes;
		}
	}
	//全部赋-1就是清空
	template<size_t n>
	void unionFindSet<n>::clear() {
		memset(parent, -1, sizeof(int) * n);
	}

}
#endif // ! __UNION_FIND_SET_

