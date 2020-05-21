#include "Cache.h"
#include <iostream>
#include <fstream>

Cache::Cache()
{
	p_3.cach_siz = (long_)0;
	p_3.block_siz = (long_)0;
	p_3.cach_type = "";
	p_3ans.Cache_size = (long_)0;
	p_3ans.Block_size = (long_)0;
	p_3ans.placement_type = "";
	p_3ans.write_policy = "";
	p_3ans.hit_rate = 0.0;
	p_3ans.memory_cache = (long_)0;
	p_3ans.cache_memory = (long_)0;
	p_3ans.block_set = (long_)0;
	data = { 0, 0 };
	mem = { 0, 0 };
	for_cach = { 1,4,64,128 };
	for_block = { 4,8,16,128 };
	for_type = { /*"FA",*/ "DM", "2W", "4W", "FA" };
	for_write = { "WB", "WT" };
}

Cache::~Cache()
{
	for_cach.clear();
	for_block.clear();
	for_type.clear();
	for_write.clear();
	clear_cache();
}

void Cache::input_to_output(const std::string& fin, const std::string& fout)
{
	std::ifstream file(fin);
	std::ofstream fole(fout);
	std::string line_in;
	if (fole.is_open() && file.is_open())
	{
		while (file.peek() != EOF) {
			std::getline(file, line_in);
			std::istringstream in_(line_in);
			std::pair<std::string, std::string> in_file;
			in_ >> in_file.first >> in_file.second;
			all_in.push_back(in_file);
		}
		file.close();
		char* p;
		for (auto a : for_cach)
		{
			for (auto b : for_block)
			{
				for (auto c : for_type)
				{
					for (auto d : for_write)
					{
						p_3.cach_siz = a;
						p_3.block_siz = b;
						p_3.cach_type = c;
						cachee = p_3.create_cache();
						p_3ans.Cache_size = a;
						p_3ans.Block_size = b;
						p_3ans.placement_type = c;
						p_3ans.write_policy = d;
						p_3ans.block_set = p_3.set_siz();
						data = { 0, 0 };
						mem = { 0, 0 };
						p_3ans.memory_cache = (long_)0;
						p_3ans.cache_memory = (long_)0;
						for (auto e : all_in)
						{
							std::stringstream iss;
							iss << std::hex << e.second;
							unsigned int x;
							iss >> x;
							std::bitset<32> addrss(x);
							std::string kev_ = addrss.to_string();
							tag = std::strtol(kev_.substr(0, p_3.tag_index()).c_str(), &p, 2);
							index = std::strtol(kev_.substr(p_3.tag_index(), p_3.set_index()).c_str(), &p, 2);

							if (d == "WB") {
								if (e.first == "read") {
									wb_read();
								}
								if (e.first == "write") {
									wb_write();
								}
							}

							if (d == "WT") {
								if (e.first == "read") {
									wt_read();
								}
								if (e.first == "write") {
									wt_write();
								}
							}
							p_3ans.hit_rate = data.hit_r();
							// p_3ans.cache_memory += mem.cach_mem(p_3ans.Block_size);
							p_3ans.memory_cache = mem.mem_cach(p_3ans.Block_size);
						}
						p_3ans.Cache_size *= 1024;
						count_dirty();
						fole << p_3ans.Cache_size << "\t" << p_3ans.Block_size << "\t" << p_3ans.placement_type << "\t" << p_3ans.write_policy << "\t" << std::setprecision(2) << p_3ans.hit_rate << "\t" << p_3ans.memory_cache << "\t" << p_3ans.cache_memory << "\t" << p_3ans.block_set << std::endl;
					}
				}
			}
		}
		fole.close();
	}
}

void Cache::wb_read()
{
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].first == tag && cachee[index][a].second.valid == 1) {
			data.hit++;
			update_cache(index, a);
			return;
		}
	}

	data.miss++;
	mem.mem_R++;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].second.valid == 0) {
			cachee[index][a].first = tag;
			cachee[index][a].second.valid = 1;
			update_cache(index, a);
			return;
		}
	}

	long_ low = 0;
	long_ high = 0;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].second.recent > high) {
			high = cachee[index][a].second.recent;
			low = a;
		}
	}

	if (cachee[index][low].second.dirty == 1) {
		//mem.mem_W++;
		p_3ans.cache_memory += p_3ans.Block_size;
		cachee[index][low].second.dirty = 0;
	}
	cachee[index][low].first = tag;
	cachee[index][low].second.valid = 1;
	update_cache(index, low);
	return;
}

void Cache::wt_read()
{
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].first == tag && cachee[index][a].second.valid == 1) {
			data.hit++;
			update_cache(index, a);
			return;
		}
	}

	data.miss++;
	mem.mem_R++;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].second.valid == 0) {
			cachee[index][a].first = tag;
			cachee[index][a].second.valid = 1;
			update_cache(index, a);
			return;
		}
	}

	long_ low = 0;
	long_ high = 0;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].second.recent > high) {
			high = cachee[index][a].second.recent;
			low = a;
		}
	}

	if (cachee[index][low].second.dirty == 1) {
		// mem.mem_W++;
		p_3ans.cache_memory += 4;
	}

	cachee[index][low].first = tag;
	cachee[index][low].second.valid = 1;
	update_cache(index, low);
}

void Cache::wb_write()
{
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].first == tag && cachee[index][a].second.valid == 1) {
			cachee[index][a].second.dirty = 1;
			data.hit++;
			update_cache(index, a);
			return;
		}
	}

	data.miss++;
	mem.mem_R++;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].second.valid == 0) {
			//mem.mem_W++;
			cachee[index][a] = { tag, {1,1,0} };
			update_cache(index, a);
			return;
		}
	}

	long_ low = 0;
	long_ high = 0;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].second.recent > high) {
			high = cachee[index][a].second.recent;
			low = a;
		}
	}

	if (cachee[index][low].second.dirty == 1) {
		//mem.mem_W++;
		p_3ans.cache_memory += p_3ans.Block_size;
	}
	cachee[index][low] = { tag, {1,1,0} };
	update_cache(index, low);
}

void Cache::wt_write()
{
	//mem.mem_W++;
	p_3ans.cache_memory += 4;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].first == tag && cachee[index][a].second.valid == 1) {
			cachee[index][a].second.dirty = 1;
			data.hit++;
			update_cache(index, a);
			return;
		}
	}

	data.miss++;
	mem.mem_R++;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].second.valid == 0) {
			cachee[index][a] = { tag, {1,1,0} };
			update_cache(index, a);
			return;
		}
	}

	long_ low = 0;
	long_ high = 0;
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		if (cachee[index][a].second.recent > high) {
			high = cachee[index][a].second.recent;
			low = a;
		}
	}

	cachee[index][low] = { tag, {1,1,0} };
	update_cache(index, low);
}

void Cache::update_cache(long_ new_, long_ inde)
{
	for (long_ a = 0; a < p_3ans.block_set; a++) {
		cachee[new_][a].second.recent++;
	}
	cachee[new_][inde].second.recent = 0;
}

void Cache::clear_cache()
{
	for (long_ i = 0; i < cachee.size(); i++) {
		cachee[i].clear();
		cachee[i].resize(0);
	}
	cachee.clear();
	cachee.resize(0);
	p_3.cach_siz = (long_)0;
	p_3.block_siz = (long_)0;
	p_3.cach_type = "";
	p_3ans.Cache_size = (long_)0;
	p_3ans.Block_size = (long_)0;
	p_3ans.placement_type = "";
	p_3ans.write_policy = "";
	p_3ans.hit_rate = 0.0;
	p_3ans.memory_cache = (long_)0;
	p_3ans.cache_memory = (long_)0;
	p_3ans.block_set = (long_)0;
	data = { 0, 0 };
	mem = { 0, 0 };
}

void Cache::count_dirty()
{
	for (long_ i = 0; i < cachee.size(); i++) {
		for (long_ j = 0; j < cachee[i].size(); j++) {
			if (cachee[i][j].second.dirty == 1)
			{
				p_3ans.cache_memory += p_3ans.Block_size;
			}
		}
	}
}