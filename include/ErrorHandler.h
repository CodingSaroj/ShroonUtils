#ifndef SUTL_ERROR_HANDLER_H
#define SUTL_ERROR_HANDLER_H

/**
 * @file ErrorHandler.h
 * This file contains the declaration/definition of the error handler function pointer.
 */

#if defined(SUTL_IMPLEMENTATION) && !(defined(SUTL_ERROR_HANDLER_CUSTOM) && SUTL_ERROR_HANDLER_CUSTOM)
    #include <stdio.h>

    void SUTL_InternalErrorHandler(const char * msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    void( * SUTLErrorHandler)(const char *) = SUTL_InternalErrorHandler;
#else
    /**
     * @brief The function which will handle errors.
     *
     * By default, it print errors to \p stderr but if \p SUTL_ERROR_HANDLER_CUSTOM is defined, then
     * this must be defined and set to a custom function otherwise linker errors will occur.
     */
    extern void( * SUTLErrorHandler)(const char *);
#endif

#endif
