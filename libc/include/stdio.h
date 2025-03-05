#ifndef _STDIO_H
#define _STDIO_H 1
 
#include <sys/cdefs.h>
#include <uchar.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
 
#define EOF (-1)
 
#ifdef __cplusplus
extern "C" {
#endif


#ifndef BUFSIZ
#define BUFSIZ 0x4000
#endif
#ifndef SEEK_SET
#define SEEK_SET    0   /* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR    1   /* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define SEEK_END    2   /* set file offset to EOF plus offset */
#endif



static const char hex_digits[30] = "0123456789ABCDEF";
static const char hex_digits_low[30] = "0123456789abcdef";

typedef enum{
    UNSET,
    NARROW,
    WIDE
} char_width_t;

typedef enum{
    NONE,
    LINE,
    FULL
} buffer_state_t;

typedef enum{
    INPUT,
    OUTPUT,
    UPDATE
} io_mode_t;

typedef uint64_t fpos_t;

// FILE
typedef struct {
    unsigned char char_width;
    mbstate_t parse_state;
    buffer_state_t buffer_state;
    io_mode_t io_mode;
    bool text_mode;
    bool eof;
    fpos_t pos;
    int buf_size;
    unsigned short error;
    bool lock;
    int descriptor;
    // array pointer
    unsigned char* buffer;
    bool dynamic_buf;
    int last_char; // rewind char
    uint64_t size;
} FILE;

extern FILE* stderr;
//#define stderr stderr

extern FILE* stdout;
//#define stdout stdout

extern FILE* stdin;
//#define stdin stdin


// Remove:
//The remove function causes the file whose name is the string pointed to by filename
// to be no longer accessible by that name. A subsequent attempt to open that file using that
// name will fail, unless it is created anew. If the file is open, the behavior of the remove
// function is implementation-defined

// return non-zero if fail
int remove(const char* filename);

// The rename function causes the file whose name is the string pointed to by old to be
// henceforth known by the name given by the string pointed to by new. The file named
// old is no longer accessible by that name. If a file named by the string pointed to by new
// exists prior to the call to the rename function, the behavior is implementation-defined.

// return non-zero if fail (if it fails the file has the same name)
int rename(const char *old, const char *new);

/*
The tmpfile function creates a temporary binary file that is different from any other
existing file and that will automatically be removed when it is closed or at program
termination. If the program terminates abnormally, whether an open temporary file is
removed is implementation-defined. The file is opened for update with "wb+" mode.
*/
FILE* tmpfile(void);

/*
The tmpnam function generates a string that is a valid file name and that is not the same
as the name of an existing file.270) The function is potentially capable of generating at
least TMP_MAX different strings, but any or all of them may already be in use by existing
files and thus not be suitable return values

returns null if it fails
*/
char *tmpnam(char* s);

/*/
A successful call to the fclose function causes the stream pointed to by stream to be
flushed and the associated file to be closed. Any unwritten buffered data for the stream
are delivered to the host environment to be written to the file; any unread buffered data
are discarded. Whether or not the call succeeds, the stream is disassociated from the file
and any buffer set by the setbuf or setvbuf function is disassociated from the stream
(and deallocated if it was automatically allocated).

The fclose function returns zero if the stream was successfully closed, or EOF if any
errors were detected.

*/
int fclose(FILE *stream);

/*
If stream points to an output stream or an update stream in which the most recent
operation was not input, the fflush function causes any unwritten data for that stream
to be delivered to the host environment to be written to the file; otherwise, the behavior is
undefined.
If stream is a null pointer, the fflush function performs this flushing action on all
streams for which the behavior is defined above.

 The fflush function sets the error indicator for the stream and returns EOF if a write
error occurs, otherwise it returns zero.
*/
int fflush(FILE *stream);

/*
The fopen function opens the file whose name is the string pointed to by filename,
and associates a stream with it.

When opened, a stream is fully buffered if and only if it can be determined not to refer to
an interactive device. The error and end-of-file indicators for the stream are cleared
*/
FILE *fopen(const char* restrict filename, const char* restrict mode);

/*
2 The freopen function opens the file whose name is the string pointed to by filename
and associates the stream pointed to by stream with it. The mode argument is used just
as in the fopen function.272)
3 If filename is a null pointer, the freopen function attempts to change the mode of
the stream to that specified by mode, as if the name of the file currently associated with
the stream had been used. It is implementation-defined which changes of mode are
permitted (if any), and under what circumstances.

The freopen function first attempts to close any file that is associated with the specified
stream. Failure to close the file is ignored. The error and end-of-file indicators for the
stream are cleared.

The freopen function returns a null pointer if the open operation fails. Otherwise,
freopen returns the value of stream.
*/
FILE *freopen(const char* restrict filename, const char* restrict mode, FILE* restrict stream);

/*
 Except that it returns no value, the setbuf function is equivalent to the setvbuf
function invoked with the values _IOFBF for mode and BUFSIZ for size, or (if buf
is a null pointer), with the value _IONBF for mode.
*/
void setbuf(FILE* restrict stream, char* restrict buf);

/*
 The setvbuf function may be used only after the stream pointed to by stream has
been associated with an open file and before any other operation (other than an
unsuccessful call to setvbuf) is performed on the stream. The argument mode
determines how stream will be buffered, as follows: _IOFBF causes input/output to be
fully buffered; _IOLBF causes input/output to be line buffered; _IONBF causes
input/output to be unbuffered. If buf is not a null pointer, the array it points to may be
used instead of a buffer allocated by the setvbuf function273) and the argument size
specifies the size of the array; otherwise, size may determine the size of a buffer
allocated by the setvbuf function. The contents of the array at any time are
indeterminate.
Returns

The setvbuf function returns zero on success, or nonzero if an invalid value is given
for mode or if the request cannot be honored.
*/
int setvbuf(FILE* restrict stream, char* restrict buf, int mode, size_t size);

/*
The fprintf function writes output to the stream pointed to by stream, under control
of the string pointed to by format that specifies how subsequent arguments are
converted for output. If there are insufficient arguments for the format, the behavior is
undefined. If the format is exhausted while arguments remain, the excess arguments are
evaluated (as always) but are otherwise ignored. The fprintf function returns when
the end of the format string is encountered.
3 The format shall be a multibyte character sequence, beginning and ending in its initial
shift state. The format is composed of zero or more directives: ordinary multibyte
characters (not %), which are copied unchanged to the output stream; and conversion
specifications, each of which results in fetching zero or more subsequent arguments,
converting them, if applicable, according to the correspon

WARNING: there are tons of crazy flags which are probably never used buf when i do
need them they are at: page 310-ish
*/
int fprintf(FILE* restrict stream, const char* restrict format, ...);

/*
The fscanf function reads input from the stream pointed to by stream, under control
of the string pointed to by format that specifies the admissible input sequences and how
they are to be converted for assignment, using subsequent arguments as pointers to the
objects to receive the converted input. If there are insufficient arguments for the format,
the behavior is undefined. If the format is exhausted while arguments remain, the excess
arguments are evaluated (as always) but are otherwise ignored.
*/
int fscanf(FILE* restrict stream, const char* restrict format, ...);

/*
The printf function is equivalent to fprintf with the argument stdout interposed
before the arguments to printf.

Returns
The printf function returns the number of characters transmitted, or a negative value if
an output or encoding error occurred.
*/
int printf(const char* restrict format, ...);

/*
 The scanf function is equivalent to fscanf with the argument stdin interposed
before the arguments to scanf.
Returns
 The scanf function returns the value of the macro EOF if an input failure occurs before
the first conversion (if any) has completed. Otherwise, the scanf function returns the
number of input items assigned, which can be fewer than provided for, or even zero, in
the event of an early matching failure.
*/
int scanf(const char* __restrict, ...);

/*
 The snprintf function is equivalent to fprintf, except that the output is written into
an array (specified by argument s) rather than to a stream. If n is zero, nothing is written,
and s may be a null pointer. Otherwise, output characters beyond the n-1st are
discarded rather than being written to the array, and a null character is written at the end
of the characters actually written into the array. If copying takes place between objects
that overlap, the behavior is undefined.
Returns

The snprintf function returns the number of characters that would have been written
had n been sufficiently large, not counting the terminating null character, or a neg ative
value if an encoding error occurred. Thus, the null-terminated output has been
completely written if and only if the returned value is nonnegative and less than n.
*/
int snprintf(char* restrict s, size_t n, const char* format, ...);

/*
The sprintf function is equivalent to fprintf, except that the output is written into
an array (specified by the argument s) rather than to a stream. A null character is written
at the end of the characters written; it is not counted as part of the returned value. If
copying takes place between objects that overlap, the behavior is undefined.
Returns
3 The sprintf function returns the number of characters written in the array, not
counting the terminating null character, or a neg ative value if an encoding error occurred.
*/
int sprintf(char* restrict s, const char* format, ...);

/*
The sscanf function is equivalent to fscanf, except that input is obtained from a
string (specified by the argument s) rather than from a stream. Reaching the end of the
string is equivalent to encountering end-of-file for the fscanf function. If copying
takes place between objects that overlap, the behavior is undefined.

The sscanf function returns the value of the macro EOF if an input failure occurs
before the first conversion (if any) has completed. Otherwise, the sscanf function
returns the number of input items assigned, which can be fewer than provided for, or even
zero, in the event of an early matching failure.
*/
int sscanf(const char* restrict s, const char* restrict format, ...);

/*
The vfprintf function is equivalent to fprintf, with the variable argument list
replaced by arg, which shall have been initialized by the va_start macro (and
possibly subsequent va_arg calls). The vfprintf function does not invoke the
va_end macro.

Returns
3 The vfprintf function returns the number of characters transmitted, or a negative
value if an output or encoding error occurred.
*/
int vfprintf(FILE* restrict stream, const char* restrict format, va_list arg);

/*
The vfscanf function is equivalent to fscanf, with the variable argument list
replaced by arg, which shall have been initialized by the va_start macro (and
possibly subsequent va_arg calls). The vfscanf function does not invoke the
va_end macro.

Returns
The vfscanf function returns the value of the macro EOF if an input failure occurs
before the first conversion (if any) has completed. Otherwise, the vfscanf function
returns the number of input items assigned, which can be fewer than provided for, or even
zero, in the event of an early matching failure
*/
int vfscanf(FILE* restrict stream, const char* restrict format, va_list arge);

/*
The vprintf function is equivalent to printf, with the variable argument list
replaced by arg, which shall have been initialized by the va_start macro (and
possibly subsequent va_arg calls). The vprintf function does not invoke the
va_end macro.

Returns
The vprintf function returns the number of characters transmitted, or a negative value
if an output or encoding error occurred
*/
int vprintf(const char* restrict format, va_list arg);

/*
Description
The vscanf function is equivalent to scanf, with the variable argument list replaced
by arg, which shall have been initialized by the va_start macro (and possibly
subsequent va_arg calls). The vscanf function does not invoke the va_end
macro.288)

Returns
The vscanf function returns the value of the macro EOF if an input failure occurs
before the first conversion (if any) has completed. Otherwise, the vscanf function
returns the number of input items assigned, which can be fewer than provided for, or even
zero, in the event of an early matching failure.
*/
int vscanf(const char* restrict format, va_list arg);

/*
The vsnprintf function is equivalent to snprintf, with the variable argument list
replaced by arg, which shall have been initialized by the va_start macro (and
possibly subsequent va_arg calls). The vsnprintf function does not invoke the
va_end macro.288) If copying takes place between objects that overlap, the behavior is
undefined.

Returns
The vsnprintf function returns the number of characters that would have been written
had n been sufficiently large, not counting the terminating null character, or a neg ative
value if an encoding error occurred. Thus, the null-terminated output has been
completely written if and only if the returned value is nonnegative and less than n
*/
int vsnprintf(char* restrict s, size_t n, const char* restrict format, va_list arg);

/*
The vsprintf function is equivalent to sprintf, with the variable argument list
replaced by arg, which shall have been initialized by the va_start macro (and
possibly subsequent va_arg calls). The vsprintf function does not invoke the
va_end macro. If copying takes place between objects that overlap, the behavior is
undefined.

Returns
The vsprintf function returns the number of characters written in the array, not
counting the terminating null character, or a negative value if an encoding error occurred.
*/
int vsprintf(char* restrict s, const char* restrict format, va_list arg);

/*
The vsscanf function is equivalent to sscanf, with the variable argument list
replaced by arg, which shall have been initialized by the va_start macro (and
possibly subsequent va_arg calls). The vsscanf function does not invoke the
va_end macro.

Returns
The vsscanf function returns the value of the macro EOF if an input failure occurs
before the first conversion (if any) has completed. Otherwise, the vsscanf function
returns the number of input items assigned, which can be fewer than provided for, or even
zero, in the event of an early matching failure.
*/
int vsscanf(const char* restrict s, const char* restrict format, va_list arg);

/*
If the end-of-file indicator for the input stream pointed to by stream is not set and a
next character is present, the fgetc function obtains that character as an unsigned
char converted to an int and advances the associated file position indicator for the
stream (if defined).

Returns
If the end-of-file indicator for the stream is set, or if the stream is at end-of-file
the endof-file indicator for the stream is set and the fgetc function returns EOF. Otherwise, the
fgetc function returns the next character from the input stream pointed to by stream.
If a read error occurs, the error indicator for the stream is set and the fgetc function
returns EOF.
*/
int fgetc(FILE* stream);

/*
The fgets function reads at most one less than the number of characters specified by n
from the stream pointed to by stream into the array pointed to by s. No additional
characters are read after a new-line character (which is retained) or after end-of-file. A
null character is written immediately after the last character read into the array.
Returns

The fgets function returns s if successful. If end-of-file is encountered and no
characters have been read into the array, the contents of the array remain unchanged and a
null pointer is returned. If a read error occurs during the operation, the array contents are
indeterminate and a null pointer is returned.
*/
char* fgets(char* restrict s, int n, FILE* restrict stream);

/*
The fputc function writes the character specified by c (converted to an unsigned
char) to the output stream pointed to by stream, at the position indicated by the
associated file position indicator for the stream (if defined), and advances the indicator
appropriately. If the file cannot support positioning requests, or if the stream was opened
with append mode, the character is appended to the output stream.

Returns
The fputc function returns the character written. If a write error occurs, the error
indicator for the stream is set and fputc returns EOF.
*/
int fputc(int c, FILE* restrict stream);

/*
The fputs function writes the string pointed to by s to the stream pointed to by
stream. The terminating null character is not written.

Returns
The fputs function returns EOF if a write error occurs; otherwise it returns a
nonnegative value.
*/
int fputs(const char* restrict s, FILE* restrict stream);

/*
The getc function is equivalent to fgetc, except that if it is implemented as a macro, it
may evaluate stream more than once, so the argument should never be an expression
with side effects.

Returns
The getc function returns the next character from the input stream pointed to by
stream. If the stream is at end-of-file, the end-of-file indicator for the stream is set and
getc returns EOF. If a read error occurs, the error indicator for the stream is set and
getc returns EOF.
*/
int getc(FILE* stream);

/*
The getchar function is equivalent to getc with the argument stdin.

Returns
The getchar function returns the next character from the input stream pointed to by
stdin. If the stream is at end-of-file, the end-of-file indicator for the stream is set and
getchar returns EOF. If a read error occurs, the error indicator for the stream is set and
getchar returns EOF.
*/
int getchar(void);

/*
The putc function is equivalent to fputc, except that if it is implemented as a macro, it
may evaluate stream more than once, so that argument should never be an expression
with side effects.

Returns
The putc function returns the character written. If a write error occurs, the error
indicator for the stream is set and putc returns EOF.
*/
int putc(int c, FILE* stream);

/*
The putchar function is equivalent to putc with the second argument stdout.

Returns
The putchar function returns the character written. If a write error occurs, the error
indicator for the stream is set and putchar returns EOF
*/
int putchar(int c);

/*
The puts function writes the string pointed to by s to the stream pointed to by stdout,
and appends a new-line character to the output. The terminating null character is not
written.

Returns
The puts function returns EOF if a write error occurs; otherwise it returns a nonnegative
value.
*/
int puts(const char *s);

/*
The ungetc function pushes the character specified by c (converted to an unsigned
char) back onto the input stream pointed to by stream. Pushed-back characters will be
returned by subsequent reads on that stream in the reverse order of their pushing. A
successful intervening call (with the stream pointed to by stream) to a file positioning
function (fseek, fsetpos, or rewind) discards any pushed-back characters for the
stream. The external storage corresponding to the stream is unchanged.
3 One character of pushback is guaranteed. If the ungetc function is called too many
times on the same stream without an intervening read or file positioning operation on that
stream, the operation may fail.
4 If the value of c equals that of the macro EOF, the operation fails and the input stream is
unchanged.
5 A successful call to the ungetc function clears the end-of-file indicator for the stream.
The value of the file position indicator for the stream after reading or discarding all
pushed-back characters shall be the same as it was before the characters were pushed
back. For a text stream, the value of its file position indicator after a successful call to the
ungetc function is unspecified until all pushed-back characters are read or discarded.
For a binary stream, its file position indicator is decremented by each successful call to
the ungetc function; if its value was zero before a call, it is indeterminate after the
call.

Returns
6 The ungetc function returns the character pushed back after conversion, or EOF if the
operation fails
*/
int ungetc(int c, FILE* stream);


/*
The fread function reads, into the array pointed to by ptr, up to nmemb elements
whose size is specified by size, from the stream pointed to by stream. For each
object, size calls are made to the fgetc function and the results stored, in the order
read, in an array of unsigned char exactly overlaying the object. The file position
indicator for the stream (if defined) is advanced by the number of characters successfully
read. If an error occurs, the resulting value of the file position indicator for the stream is
indeterminate. If a partial element is read, its value is indeterminate.

Returns
The fread function returns the number of elements successfully read, which may be
less than nmemb if a read error or end-of-file is encountered. If size or nmemb is zero,
fread returns zero and the contents of the array and the state of the stream remain
unchanged.
*/
size_t fread(void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream);

/*
the fwrite function writes, from the array pointed to by ptr, up to nmemb elements
whose size is specified by size, to the stream pointed to by stream. For each object,
size calls are made to the fputc function, taking the values (in order) from an array of
unsigned char exactly overlaying the object. The file position indicator for the
stream (if defined) is advanced by the number of characters successfully written. If an
error occurs, the resulting value of the file position indicator for the stream is
indeterminate.

Returns
The fwrite function returns the number of elements successfully written, which will be
less than nmemb only if a write error is encountered. If size or nmemb is zero,
fwrite returns zero and the state of the stream remains unchanged.
*/
size_t fwrite(const void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream);

/*
The fgetpos function stores the current values of the parse state (if any) and file
position indicator for the stream pointed to by stream in the object pointed to by pos.
The values stored contain unspecified information usable by the fsetpos function for
repositioning the stream to its position at the time of the call to the fgetpos function.

Returns
If successful, the fgetpos function returns zero; on failure, the fgetpos function
returns nonzero and stores an implementation-defined positive value in errno.
*/
int fgetpos(FILE* restrict stream, fpos_t pos);

/*
The fseek function sets the file position indicator for the stream pointed to by stream.
If a read or write error occurs, the error indicator for the stream is set and fseek fails

Returns
The fseek function returns nonzero only for a request that cannot be satisfied
*/
int fseek(FILE *stream, long int offset, int whence);

/*
The fsetpos function sets the mbstate_t object (if any) and file position indicator
for the stream pointed to by stream according to the value of the object pointed to by
pos, which shall be a value obtained from an earlier successful call to the fgetpos
function on a stream associated with the same file. If a read or write error occurs, the
error indicator for the stream is set and fsetpos fails.

A successful call to the fsetpos function undoes any effects of the ungetc function
on the stream, clears the end-of-file indicator for the stream, and then establishes the new
parse state and position. After a successful fsetpos call, the next operation on an
update stream may be either input or output.

Returns
If successful, the fsetpos function returns zero; on failure, the fsetpos function
returns nonzero and stores an implementation-defined positive value in errno.
*/
int fsetpos(FILE *stream, const fpos_t *pos);

/*
The ftell function obtains the current value of the file position indicator for the stream
pointed to by stream. For a binary stream, the value is the number of characters from
the beginning of the file. For a text stream, its file position indicator contains unspecified
information, usable by the fseek function for returning the file position indicator for the
stream to its position at the time of the ftell call; the difference between two such
return values is not necessarily a meaningful measure of the number of characters written
or read.

Returns
If successful, the ftell function returns the current value of the file position indicator
for the stream. On failure, the ftell function returns −1L and stores an
implementation-defined positive value in errno.
*/
long int ftell(FILE *stream);


/*
The rewind function sets the file position indicator for the stream pointed to by
stream to the beginning of the file. It is equivalent to
(void)fseek(stream, 0L, SEEK_SET)
except that the error indicator for the stream is also cleared.

Returns
The rewind function returns no value.
*/
void rewind(FILE* stream);

/*
Description
The clearerr function clears the end-of-file and error indicators for the stream pointed
to by stream.
Returns
The clearerr function returns no value.
*/
void clearerr(FILE* stream);

/*
The feof function tests the end-of-file indicator for the stream pointed to by stream.

Returns
The feof function returns nonzero if and only if the end-of-file indicator is set for
stream.
*/
int feof(FILE* stream);

/*
The ferror function tests the error indicator for the stream pointed to by stream.

Returns
The ferror function returns nonzero if and only if the error indicator is set for
stream
*/
int ferror(FILE* stream);

/*
The perror function maps the error number in the integer expression errno to an
error message. It writes a sequence of characters to the standard error stream thus: first
(if s is not a null pointer and the character pointed to by s is not the null character), the
string pointed to by s followed by a colon (:) and a space; then an appropriate error
message string followed by a new-line character. The contents of the error message
strings are the same as those returned by the strerror function with argument errno.

Returns
The perror function returns no value.
*/
void perror(const char* s);


#ifdef __cplusplus
}
#endif
 
#endif