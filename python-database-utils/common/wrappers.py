import logging


class logger(object):
    def __init__(self, func):
        self.func = func
    def __get__(self, obj, type=None):
        return self.__class__(self.func.__get__(obj, type))
    def __call__(self, *args, **kw):
        logging.info('Entering %s' % self.func)
        return self.func(*args, **kw)


class general_function_handler(object):
    def __init__(self, func):
        self.func = func
    def __get__(self, obj, type=None):
        return self.__class__(self.func.__get__(obj, type))
    def __call__(self, *args, **kw):
        try:
            retval = self.func(*args, **kw)
        except Exception, e :
            logging.warning('Exception in %s' % self.func)
            template = "An exception of type {0} occured. Arguments:\n{1!r}"
            message = template.format(type(e).__name__, e.args)
            logging.exception(message)
            retval = None
            # probably should sys.exit(1) for certain exception types
            # like AttributeError, KeyError, NameError
        return retval
    
class database_function_handler(object):
    def __init__(self, func):
        self.func = func
    def __get__(self, obj, type=None):
        return self.__class__(self.func.__get__(obj, type))
    def __call__(self, conn, *args, **kw):
        cursor = conn.cursor()
        try:
            cursor.execute("BEGIN")
            retval = self.func(cursor, *args, **kw)
            cursor.execute("COMMIT")
        except Exception, e :
            logging.warning('Exception in %s' % self.func)
            template = "An exception of type {0} occured. Arguments:\n{1!r}"
            message = template.format(type(e).__name__, e.args)
            logging.exception(message)
            cursor.execute("ROLLBACK")
            retval = None
        finally:
            cursor.close()

        return retval
    

def db_trx_exception_handler(func):
    """Wrap a function in an idomatic SQL transaction.  The wrapped function
    should take a cursor as its first argument; other arguments will be
    preserved.
    """

    def new_func(conn, *args, **kwargs):

        retval = None
        cursor = conn.cursor()
        try:
            cursor.execute("BEGIN")
            retval = func(cursor, *args, **kwargs)
            cursor.execute("COMMIT")
        except Exception, e:
            cursor.execute("ROLLBACK")
            logging.warning("Database Exception Handled:\n " + str(e))
        finally:
            cursor.close()

        return retval

    # Tidy up the help()-visible docstrings to be nice
    new_func.__name__ = func.__name__
    new_func.__doc__ = func.__doc__

    return new_func


def general_exception_handler(func):
    
    def new_func(config, *args, **kwargs):
        retval = None
        try:
            retval = func(config, *args, **kwargs)
        except Exception, e:
            logging.warning("General Exception Handled:\n " + str(e))
        
        return retval

    # Tidy up the help()-visible docstrings to be nice
    new_func.__name__ = func.__name__
    new_func.__doc__ = func.__doc__
        
