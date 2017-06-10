def html():
    return "text"

def make_bold(f):
    def wrapper(): 
        return "<b>" + f() + "</b>"
    return wrapper

def make_italic(f):
    def wrapper(): 
        return "<i>" + f() + "</i>"
    return wrapper

html = make_bold(html) 
html = make_italic(html)
print html()
