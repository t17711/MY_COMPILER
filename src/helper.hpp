#ifndef HELPER_HPP
#define HELPER_HPP

#include <sstream>
	
	inline void error_e(std::ostream& o){
		o<<std::endl;
	}

	template<typename F, typename ...VALUES>
	inline void error_e(std::ostream& o, const F& val,  const VALUES & ...rest){
		o<<val<<" ";;
		return error_e(o,rest...);
	}


	template<typename ...VALUES>
	inline void error(const VALUES & ...rest){
		std::ostringstream oss;
		error_e(oss, rest...);
		std::cout << oss.str();
		exit(EXIT_FAILURE);

	}


#endif