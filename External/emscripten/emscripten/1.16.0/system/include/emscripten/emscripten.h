#ifndef __emscripten_h__
#define __emscripten_h__

/**
 * This file contains a few useful things for compiling C/C++ code
 * with Emscripten, an LLVM-to-JavaScript compiler.
 *
 * The code can be used permissively under the MIT license.
 *
 * http://emscripten.org
 */

#ifdef __cplusplus
extern "C" {
#endif

#if !__EMSCRIPTEN__
#include <SDL/SDL.h> /* for SDL_Delay in async_call */
#endif

/*
 * Convenient syntax for inline assembly/js. Allows stuff like
 *
 *    EM_ASM(window.alert('hai'));
 *
 * This also works with asm.js, as it outlines the code (it
 * does a function call to reach it). It supports newlines,
 *
 *    EM_ASM(
 *      window.alert('hai'));
 *      window.alert('bai'));
 *    )
 *
 * Notes: Double-quotes (") are not supported, but you can use
 *        single-quotes (') in js anyhow.
 *
 *        You can't access C variables with EM_ASM, nor receive
 *        a value back. use EM_ASM_INT or EM_ASM_DOUBLE for that
 */
#define EM_ASM(...) emscripten_asm_const(#__VA_ARGS__)

/*
 * Input-output versions of EM_ASM.
 *
 * EM_ASM_ (an extra _ is added) or EM_ASM_ARGS allow sending values
 * (ints or doubles) into the code. If you also want a return value,
 * EM_ASM_INT receives arguments (of int or double type)
 * and returns an int; EM_ASM_DOUBLE does the same and returns
 * a double.
 *
 * Arguments arrive as $0, $1 etc. The output value should be returned:
 *
 *    int x = EM_ASM_INT({
 *      console.log('I received: ' + [$0, $1]);
 *      return $0 + $1;
 *    }, calc(), otherCalc());
 *
 * Note the { and }. If you just want to receive an output value
 * (int or double) but *not* to pass any values, you can use
 * EM_ASM_INT_V and EM_ASM_DOUBLE_V respectively.
 */
#define EM_ASM_(code, ...) emscripten_asm_const_int(#code, __VA_ARGS__)
#define EM_ASM_ARGS(code, ...) emscripten_asm_const_int(#code, __VA_ARGS__)
#define EM_ASM_INT(code, ...) emscripten_asm_const_int(#code, __VA_ARGS__)
#define EM_ASM_DOUBLE(code, ...) emscripten_asm_const_double(#code, __VA_ARGS__)
#define EM_ASM_INT_V(code) emscripten_asm_const_int(#code)
#define EM_ASM_DOUBLE_V(code) emscripten_asm_const_double(#code)

/*
 * Forces LLVM to not dead-code-eliminate a function. This also
 * exports the function, as if you added it to EXPORTED_FUNCTIONS.
 */
#define EMSCRIPTEN_KEEPALIVE __attribute__((used))

/*
 * Interface to the underlying JS engine. This function will
 * eval() the given script.
 */
extern void emscripten_run_script(const char *script);
extern int emscripten_run_script_int(const char *script);
extern char *emscripten_run_script_string(const char *script); // uses a single buffer - shared between calls!

/*
 * Asynchronously run a script, after a specified amount of
 * time.
 */
extern void emscripten_async_run_script(const char *script, int millis);

/*
 * Asynchronously loads a script from a URL.
 *
 * This integrates with the run dependencies system, so your
 * script can call addRunDependency multiple times, prepare
 * various asynchronous tasks, and call removeRunDependency
 * on them; when all are complete (or there were no run
 * dependencies to begin with), onload is called. An example use
 * for this is to load an asset module, that is, the output of the
 * file packager.
 */
extern void emscripten_async_load_script(const char *script, void (*onload)(void), void (*onerror)(void));

/*
 * Set a C function as the main event loop. The JS environment
 * will call that function at a specified number of frames per
 * second. Setting 0 or a negative value as the fps will use
 * the browser's requestAnimationFrame mechanism. This is
 * *HIGHLY* recommended if you are doing rendering, as
 * the browser's requestAnimationFrame will make sure you
 * render at a proper smooth rate that lines up with the
 * the browser and monitor in a proper way. (If you do not
 * render at all in your application, then you should pick a
 * specific frame rate that makes sense for your code.)
 *
 * Pausing and resuming the main loop is useful if your app
 * needs to perform some synchronous operation, for example
 * to load a file from the network. It might be wrong to
 * run the main loop before that finishes (the original
 * code assumes that), so you can break the code up into
 * asynchronous callbacks, but you must pause the main
 * loop until they complete.
 *
 * If you want your main loop function to receive a void*
 * argument, use emscripten_set_main_loop_arg.

 * @simulate_infinite_loop If true, this function will throw an
 *    exception in order to stop execution of the caller. This
 *    will lead to the main loop being entered instead of code
 *    after the call to emscripten_set_main_loop being run, which
 *    is the closest we can get to simulating an infinite loop
 *    (we do something similar in glutMainLoop in GLUT). If this
 *    parameter is false, then the behavior is the same as it
 *    was before this parameter was added to the API, which is
 *    that execution continues normally. Note that in both cases
 *    we do not run global destructors, atexit, etc., since we
 *    know the main loop will still be running, but if we do
 *    not simulate an infinite loop then the stack will be unwound.
 *    That means that if simulate_infinite_loop is false, and
 *    you created an object on the stack, it will be cleaned up
 *    before the main loop will be called the first time.
 */
#if __EMSCRIPTEN__
extern void emscripten_set_main_loop(void (*func)(void), int fps, int simulate_infinite_loop);
extern void emscripten_set_main_loop_arg(void (*func)(void*), void *arg, int fps, int simulate_infinite_loop);
extern void emscripten_pause_main_loop(void);
extern void emscripten_resume_main_loop(void);
extern void emscripten_cancel_main_loop(void);
#else
#define emscripten_set_main_loop(func, fps, simulateInfiniteLoop) \
  while (1) { func(); usleep(1000000/fps); }
#define emscripten_cancel_main_loop() exit(1);
#endif

/*
 * Add a function to a queue of events that will execute
 * before the main loop will continue. The event is pushed
 * into the back of the queue. (Note that in the native version
 * of this we simply execute the function, so to keep semantics
 * identical be careful to not push while the queue is being
 * used.)
 *
 * Main loop blockers block the main loop from running, and
 * can be counted to show progress. In contrast, emscripten_async_calls
 * are not counted, do not block the main loop, and can fire
 * at specific time in the future.
 */
#if __EMSCRIPTEN__
extern void _emscripten_push_main_loop_blocker(void (*func)(void *), void *arg, const char *name);
extern void _emscripten_push_uncounted_main_loop_blocker(void (*func)(void *), void *arg, const char *name);
#else
inline void _emscripten_push_main_loop_blocker(void (*func)(void *), void *arg, const char *name) {
  func(arg);
}
inline void _emscripten_push_uncounted_main_loop_blocker(void (*func)(void *), void *arg, const char *name) {
  func(arg);
}
#endif
#define emscripten_push_main_loop_blocker(func, arg) \
  _emscripten_push_main_loop_blocker(func, arg, #func)
#define emscripten_push_uncounted_main_loop_blocker(func, arg) \
  _emscripten_push_uncounted_main_loop_blocker(func, arg, #func)

/*
 * Sets the number of blockers remaining until some user-relevant
 * event. This affects how we show progress. So if you set this
 * to 10, then push 10 blockers, as they complete the user will
 * see x/10 and so forth.
 */
#if __EMSCRIPTEN__
extern void emscripten_set_main_loop_expected_blockers(int num);
#else
inline void emscripten_set_main_loop_expected_blockers(int num) {}
#endif

/*
 * Call a C function asynchronously, that is, after returning
 * control to the JS event loop. This is done by a setTimeout.
 * When building natively this becomes a simple direct call,
 * after SDL_Delay (you must include SDL.h for that).
 *
 * If millis is negative, the browser's requestAnimationFrame
 * mechanism is used.
 */
#if __EMSCRIPTEN__
extern void emscripten_async_call(void (*func)(void *), void *arg, int millis);
#else
inline void emscripten_async_call(void (*func)(void *), void *arg, int millis) {
  if (millis) SDL_Delay(millis);
  func(arg);
}
#endif

/*
 * Exits the program immediately, but leaves the runtime alive
 * so that you can continue to run code later (so global destructors
 * etc. are not run). This is implicitly performed when you do
 * an asynchronous operation like emscripten_async_call.
 */
extern void emscripten_exit_with_live_runtime(void);

/*
 * Hide the OS mouse cursor over the canvas. Note that SDL's
 * SDL_ShowCursor command shows and hides the SDL cursor, not
 * the OS one. This command is useful to hide the OS cursor
 * if your app draws its own cursor.
 */
void emscripten_hide_mouse(void);

/*
 * Resizes the pixel width and height of the <canvas> element
 * on the Emscripten web page.
 */
void emscripten_set_canvas_size(int width, int height);

/*
 * Get the current pixel width and height of the <canvas> element
 * as well as whether the canvas is fullscreen or not.
 */
void emscripten_get_canvas_size(int *width, int *height, int *isFullscreen);

/*
 * Returns the highest-precision representation of the
 * current time that the browser provides. This uses either
 * Date.now or performance.now. The result is *not* an
 * absolute time, and is only meaningful in comparison to
 * other calls to this function. The unit is ms.
 */
#if __EMSCRIPTEN__
double emscripten_get_now(void);
#else
#include <time.h>
double emscripten_get_now(void) {
  return (1000*clock())/(double)CLOCKS_PER_SEC;
}
#endif

/*
 * Simple random number generation in [0, 1), maps to Math.random().
 */
float emscripten_random(void);

/*
 * This macro-looking function will cause Emscripten to
 * generate a comment in the generated code.
 * XXX This is deprecated for now, because it requires us to
 *     hold all global vars in memory. We need a better solution.
 */
//extern void EMSCRIPTEN_COMMENT(const char *text);

/*
 * Emscripten file system api
 */

/*
 * Load file from url in asynchronous way. In addition to
 * fetching the URL from the network, the contents are
 * prepared so that the data is usable in IMG_Load and
 * so forth (we asynchronously do the work to make the
 * browser decode the image or audio and so forth).
 * When file is ready then 'onload' callback will called.
 * If any error occurred 'onerror' will called.
 * The callbacks are called with the file as their argument.
 */
void emscripten_async_wget(const char* url, const char* file, void (*onload)(const char*), void (*onerror)(const char*));

/*
 * Data version of emscripten_async_wget. Instead of writing
 * to a file, it writes to a buffer directly in memory.
 * This avoids the overhead of using the emulated
 * filesystem, note however that since files are not used,
 * It cannot do the 'prepare' stage to set things up for
 * IMG_Load and so forth (IMG_Load etc. work on files).
 *
 * @param arg User-defined data that is passed to the callbacks,
 *
 * @param onload Callback on success, with the @arg that
 *               was provided to this function, a pointer
 *               to a buffer with the data, and the size
 *               of the buffer. As in the worker API, the
 *               data buffer only lives during the
 *               callback, so you should use it or copy
 *               it during that time and not later.
 *
 * @param onerror An optional callback on failure, with the
 *                @arg that was provided to this function.
 *
 */
void emscripten_async_wget_data(const char* url, void *arg, void (*onload)(void*, void*, int), void (*onerror)(void*));

/*
 * More feature-complete version of emscripten_async_wget. Note:
 * this version is experimental.
 *
 * The requesttype is 'GET' or 'POST',
 * If is post request, param is the post parameter 
 * like key=value&key2=value2.
 * The param 'arg' is a pointer will be pass to the callback
 * When file is ready then 'onload' callback will called.
 * During the download 'onprogress' callback will called.
 * If any error occurred 'onerror' will called.
 * The callbacks are called with an object pointer give in parameter 
 * and file if is a success, the progress value during progress
 * and http status code if is an error.
 */
void emscripten_async_wget2(const char* url, const char* file,  const char* requesttype, const char* param, void *arg, void (*onload)(void*, const char*), void (*onerror)(void*, int), void (*onprogress)(void*, int));

/*
 * More feature-complete version of emscripten_async_wget_data. Note:
 * this version is experimental.
 *
 * The requesttype is 'GET' or 'POST',
 * If is post request, param is the post parameter 
 * like key=value&key2=value2.
 * The param 'arg' is a pointer will be pass to the callback
 * The free param tells the runtime whether to free the returned buffer
   after onload is complete. If false freeing the buffer is the receiver's
   responsibility.
 * The callbacks are called with an object pointer give in parameter.
 * When file is ready then 'onload' callback will called with a pointer to
   the buffer in memory and the size in bytes.
 * During the download 'onprogress' callback will called. The first argument is
   the number of bytes loaded. The second argument is the total size in bytes,
   or zero if the size is unavailable.
 * If any error occurred 'onerror' will called with the HTTP status code
   and a string with the status description.
 */
void emscripten_async_wget2_data(const char* url, const char* requesttype, const char* param, void *arg, int free, void (*onload)(void*, void*, unsigned), void (*onerror)(void*, int, const char*), void (*onprogress)(void*, int, int));

/*
 * Prepare a file in asynchronous way. This does just the
 * preparation part of emscripten_async_wget, that is, it
 * works on file data already present, and asynchronously
 * prepares it for use in IMG_Load, Mix_LoadWAV, etc.
 * When file is loaded then 'onload' callback will called.
 * If any error occurred 'onerror' will called.
 * The callbacks are called with the file as their argument.
 * @return 0 if successful, -1 if the file does not exist
 */
int emscripten_async_prepare(const char* file, void (*onload)(const char*), void (*onerror)(const char*));

/*
 * Data version of emscripten_async_prepare, which receives
 * raw data as input instead of a filename (this can prevent
 * the need to write data to a file first). onload and
 * onerror are called back with the given arg pointer as the
 * first parameter. onload also receives a second
 * parameter, which is a 'fake' filename which you can
 * then pass into IMG_Load (it is not an actual file,
 * but it identifies this image for IMG_Load to be able
 * to process it). Note that the user of this API is
 * responsible for free()ing the memory allocated for
 * the fake filename.
 * @suffix The file suffix, e.g. 'png' or 'jpg'.
 */
void emscripten_async_prepare_data(char* data, int size, const char *suffix, void *arg, void (*onload)(void*, const char*), void (*onerror)(void*));

/*
 * Worker API. Basically a wrapper around web workers, lets
 * you create workers and communicate with them.

 * Note that the current API is mainly focused on a main thread that
 * sends jobs to workers and waits for responses, i.e., in an
 * asymmetrical manner, there is no current API to send a message
 * without being asked for it from a worker to the main thread.
 *
 */

typedef int worker_handle;

/*
 * Create and destroy workers. A worker must be compiled separately
 * from the main program, and with the BUILD_AS_WORKER flag set to 1.
 */
worker_handle emscripten_create_worker(const char *url);
void emscripten_destroy_worker(worker_handle worker);

/*
 * Asynchronously call a worker.
 *
 * The worker function will be called with two parameters: a
 * data pointer, and a size.  The data block defined by the
 * pointer and size exists only during the callback and
 * _cannot_ be relied upon afterwards - if you need to keep some
 * of that information around, you need to copy it to a safe
 * location.
 *
 * The called worker function can return data, by calling
 * emscripten_worker_respond(). If called, and if a callback was
 * given, then the callback will be called with three arguments:
 * a data pointer, a size, and  * an argument that was provided
 * when calling emscripten_call_worker (to more easily associate
 * callbacks to calls). The data block defined by the data pointer
 * and size behave like the data block in the worker function -
 * it exists only during the callback.
 *
 * @funcname the name of the function in the worker. The function
 *           must be a C function (so no C++ name mangling), and
 *           must be exported (EXPORTED_FUNCTIONS).
 * @data the address of a block of memory to copy over
 * @size the size of the block of memory
 * @callback the callback with the response (can be null)
 * @arg an argument to be passed to the callback
 */
void emscripten_call_worker(worker_handle worker, const char *funcname, char *data, int size, void (*callback)(char *, int, void*), void *arg);

/*
 * Sends a response when in a worker call. Both functions post a message
 * back to the thread which called the worker.  The _respond_provisionally
 * variant can be invoked multiple times, which will queue up messages to
 * be posted to the worker's creator.  Eventually, the _respond variant must
 * be invoked, which will disallow further messages and free framework
 * resources previously allocated for this worker call. (Calling the
 * provisional version is optional, but you must call the non-provisional
 * one to avoid leaks.) 
 */
void emscripten_worker_respond(char *data, int size);
void emscripten_worker_respond_provisionally(char *data, int size);

/*
 * Checks how many responses are being waited for from a worker. This
 * only counts calls to emscripten_call_worker that had a non-null
 * callback (if it's null, we do not have any tracking of a response),
 * and that the response was not yet received. It is a simple way to
 * check on the status of the worker to see how busy it is, and do
 * basic decisions about throttling.
 */
int emscripten_get_worker_queue_size(worker_handle worker);

/*
 * Select the networking backend to use. By default emscripten's
 * socket/networking implementation will use websockets, with this
 * function you can change that to WebRTC.
 * This function must be called before any network functions are
 * called.
 */
#define EMSCRIPTEN_NETWORK_WEBSOCKETS 0
#define EMSCRIPTEN_NETWORK_WEBRTC     1
void emscripten_set_network_backend(int backend);

/*
 * Returns the value of a compiler setting. For example
 *
 *   emscripten_get_compiler_setting("PRECISE_F32")
 *
 * will return an integer representing the value of
 * PRECISE_F32 during compilation. For values containing
 * anything other than an integer, a string is returned
 * (you will need to cast the int return value to a char*).
 *
 * Some useful things this can do is provide the
 * version of emscripten ("EMSCRIPTEN_VERSION"), the optimization
 * level ("OPT_LEVEL"), debug level ("DEBUG_LEVEL"), etc.
 *
 * For this command to work, you must build with
 *   -s RETAIN_COMPILER_SETTINGS=1
 * as otherwise we do not want to increase the build size
 * with this metadata.
 */
int emscripten_get_compiler_setting(const char *name);

/*
 * Emits
 *   debugger;
 * inline in the code, which tells the JS engine to invoke
 * the debugger if it gets there.
 */
void emscripten_debugger();


/* ===================================== */
/* Internal APIs. Be careful with these. */
/* ===================================== */

/*
 * Profiling tools.
 * INIT must be called first, with the maximum identifier that
 * will be used. BEGIN will add some code that marks
 * the beginning of a section of code whose run time you
 * want to measure. END will finish such a section. Note: If you
 * call begin but not end, you will get invalid data!
 * The profiling data will be written out if you call Profile.dump().
 */
extern void EMSCRIPTEN_PROFILE_INIT(int max);
extern void EMSCRIPTEN_PROFILE_BEGIN(int id);
extern void EMSCRIPTEN_PROFILE_END(int id);

/*
 * jcache-friendly printf. printf in general will receive a string
 * literal, which becomes a global constant, which invalidates all
 * jcache entries. emscripten_jcache_printf is parsed before
 * clang into something without any string literals, so you can
 * add such printouts to your code and only the (chunk containing
 * the) function you modify will be invalided and recompiled.
 *
 * Note in particular that you need to already have a call to this
 * function in your code *before* you add one and do an incremental
 * build, so that adding an external reference does not invalidate
 * everything.
 *
 * This function assumes the first argument is a string literal
 * (otherwise you don't need it), and the other arguments, if any,
 * are neither strings nor complex expressions (but just simple
 * variables). (You can create a variable to store a complex
 * expression on the previous line, if necessary.)
 */
#ifdef __cplusplus
void emscripten_jcache_printf(const char *format, ...);
void emscripten_jcache_printf_(...); /* internal use */
#endif

/* Helper API for EM_ASM - do not call this yourself */
void emscripten_asm_const(const char *code);
int emscripten_asm_const_int(const char *code, ...);
double emscripten_asm_const_double(const char *code, ...);

/* If specified, logs directly to the browser console/inspector 
 * window. If not specified, logs via the application Module. */
#define EM_LOG_CONSOLE   1
/* If specified, prints a warning message. */
#define EM_LOG_WARN      2
/* If specified, prints an error message. If neither EM_LOG_WARN 
 * or EM_LOG_ERROR is specified, an info message is printed.
 * EM_LOG_WARN and EM_LOG_ERROR are mutually exclusive. */
#define EM_LOG_ERROR     4
/* If specified, prints a callstack that contains filenames referring 
 * to original C sources using source map information. */
#define EM_LOG_C_STACK   8
/* If specified, prints a callstack that contains filenames referring
 * to lines to the built .js/.html file along with the message. The 
 * flags EM_LOG_C_STACK and EM_LOG_JS_STACK can be combined to output 
 * both untranslated and translated file+line information. */
#define EM_LOG_JS_STACK 16
/* If specified, C/C++ function names are demangled before printing. 
 * Otherwise, the mangled post-compilation JS function names are 
 * displayed. */
#define EM_LOG_DEMANGLE 32
/* If specified, the pathnames of the file information in the call 
 * stack will be omitted. */
#define EM_LOG_NO_PATHS 64
/* If specified, prints out the actual values of the parameters the 
 * functions were invoked with. */
#define EM_LOG_FUNC_PARAMS 128

/*
 * Prints out a message to the console, optionally with the 
 * callstack information.
 * @param flags A binary OR of items from the list of EM_LOG_xxx 
 *                 flags that specify printing options.
 * @param '...' A printf-style "format, ..." parameter list that 
 *                 is parsed according to the printf formatting rules.
 */
void emscripten_log(int flags, ...);

/*
 * Programmatically obtains the current callstack.
 * @param flags    A binary OR of items from the list of EM_LOG_xxx 
 *                    flags that specify printing options. The 
 *                    items EM_LOG_CONSOLE, EM_LOG_WARN and 
 *                    EM_LOG_ERROR do not apply in this function and 
 *                    are ignored.
 * @param out      A pointer to a memory region where the callstack 
 *                    string will be written to. The string outputted 
 *                    by this function will always be null-terminated.
 * @param maxbytes The maximum number of bytes that this function can
 *                    write to the memory pointed to by 'out'. If 
 *                    there is no enough space, the output will be 
 *                    truncated (but always null-terminated).
 * @return Returns the number of bytes written. (not number of 
 *         characters, so this will also include the terminating zero)
 
 * To query the amount of bytes needed for a callstack without writing 
 * it, pass 0 to 'out' and 'maxbytes', in which case the function will
 * return the number of bytes (including the terminating zero) that 
 * will be needed to hold the full callstack. Note that this might be 
 * fully accurate since subsequent calls will carry different line 
 * numbers, so it is best to allocate a few bytes extra to be safe.
 */
int emscripten_get_callstack(int flags, char *out, int maxbytes);

#ifdef __cplusplus
}
#endif

#endif // __emscripten_h__

