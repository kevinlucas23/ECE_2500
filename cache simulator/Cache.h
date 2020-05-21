#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <list>
#include <stdio.h>
#include <map>

typedef unsigned long long_;

struct validity {
	int valid;
	int dirty;
	long_ recent;
	bool operator==(const validity& p) const {
		return recent == p.recent;
	}
	bool operator!=(const validity& p) const {
		return recent != p.recent;
	}
	bool operator>(const validity& p) const {
		return recent > p.recent;
	}
};

struct memory {
	int mem_R;
	int mem_W;
	long_ mem_cach(const long_& a) {
		return (long_)mem_R * a;
	}
	long_ cach_mem(const long_& a) {
		return (long_)mem_W * a;
	}
};

struct for_count {
	double hit;
	double miss;
	double hit_r() {
		return (hit / (hit + miss));
	}
};

struct cache_infra {
	long_ cach_siz;
	long_ block_siz;
	std::string cach_type;
	long_ set_siz() {
		if (cach_type == "DM") {
			return (long_)1;
		}
		if (cach_type == "2W") {
			return (long_)2;
		}
		if (cach_type == "4W") {
			return (long_)4;
		}
		if (cach_type == "FA") {
			return (long_)(cach_siz * pow(2, 10)) / block_siz;
		}
		return (long_)0;
	}
	long_ off_set() {
		return (long_)log2(block_siz);
	}
	long_ set_index() {
		return (long_)log2((cach_siz * pow(2, 10)) / ((block_siz)*set_siz()));
	}
	long_ tag_index() {
		return (32 - set_index() - off_set());
	}
	std::vector<std::vector<std::pair<long_, validity>>> create_cache() {
		std::vector<std::vector<std::pair<long_, validity>>> kevin;
		long_ c = (long_)pow(2, set_index());
		kevin.resize(c);
		for (long_ i = 0; i < c; i++) {
			kevin[i].resize(set_siz());
			for (long_ j = 0; j < kevin[i].size(); j++) {
				kevin[i][j] = { 0, {0,0,0} };
			}
		}
		return kevin;
	}
};

struct final_ans {
	long_ Cache_size;
	long_ Block_size;
	std::string placement_type;
	std::string write_policy;
	double hit_rate;
	long_ memory_cache;
	long_ cache_memory;
	long_ block_set;
};

class Cache {
public:
	Cache();
	~Cache();
	void input_to_output(const std::string& fin, const std::string& fout);
	void wb_read();
	void wt_read();
	void wb_write();
	void wt_write();
	void update_cache(long_ new_, long_ inde);
	void clear_cache();
	void count_dirty();

private:
	cache_infra p_3;
	final_ans p_3ans;
	long_ offset;
	long_ tag;
	long_ index;
	memory mem;
	for_count data;
	std::vector<int> for_cach;
	std::vector<int> for_block;
	std::vector<std::string> for_type;
	std::vector<std::string> for_write;
	std::list<std::pair<std::string, std::string>> all_in;
	std::vector<std::vector<std::pair<long_, validity>>> cachee;
};