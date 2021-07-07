#ifndef __LIBIOM_H_
#define __LIBIOM_H_

#include "../src/shell.h"

#ifndef __STRING
#define __STRING #x
#endif

#define __STRING_NAME(x)     __STRING(x)

int printfs(const char *fmt, ...);
int vsprintfs(char *buffer, const char *fmt, ...);
int systems(char *cmd);


#endif /* !__LIBIOM_H_ */
