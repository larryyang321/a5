#ifndef AUTO_SIGNAL
#define AUTO_SIGNAL


#define AUTOMATIC_SIGNAL uCondition blocked

#define WAITUNTIL( pred, before, after ) \
	if(!pred){ \
		before;\
		blocked.wait();\
		after;\
	}

#define RETURN(expr...)\
	if(!blocked.empty())blocked.signal();\
	return expr;


#endif
