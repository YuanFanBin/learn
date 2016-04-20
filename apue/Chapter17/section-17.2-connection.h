/*
 * 参考资料：http://man7.org/linux/man-pages/man7/unix.7.html
 * (man 7 unix)
 */

#define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nxy.socket"
#define BUFFER_SIZE 12

/*
 * $ ./server &
 * [1] xxxx
 * $ ./client 3 4
 * Result = 7
 * $ ./client 11 -5
 * Result = 6
 * $ ./client Down
 * Result = 0
 * [1]+ Done
 * $
 */
