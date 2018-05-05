/* Header file for nJson */

#ifndef _NJSON_H_
#define _NJSON_H_

typedef struct _nJSon {
	char* name;
	void* value;
} nJson;

nJson* njson_init(nJson* this);

void njson_release(nJson* this);

#endif /* _NJSON_H_ */
