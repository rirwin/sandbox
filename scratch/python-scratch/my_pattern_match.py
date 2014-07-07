#!/usr/bin/python

'''
From careercup.com
Write a pattern matching function using wild char 

? Matches any char exactly one instance 
* Matches one or more instances of previous char 

Ex text = "abcd" pattern = "ab?d" True 
Ex text = "abccdddef" pattern = "ab?*f" True 
Ex text = "abccd" pattern = "ab?*ccd" false 

(added missing cases, making assumptions for ambiguity)
Ex text = "abcd" pattern = "a*cd" False
Ex text = "abbbbcd" pattern = "ab*cd" True
Ex text = "aabbccdd" pattern = "a?*" and "?*" True

Not sure what to do with '?*?', ignoring 
'''

def pattern_match(text, pattern):
    
    if pattern[0] == '*':
        return False  # invalid pattern

    i_txt = 0
    i_pat = 0

    prv_ch = ''

    while (i_txt < len(text)):
        if pattern[i_pat] not in ('?','*'):
            if pattern[i_pat] != text[i_txt]:
                return False
            prv_ch = text[i_txt] # save in case of '*' next
            i_pat += 1
            i_txt += 1
        elif pattern[i_pat] == '?':
            prv_ch = '?' # save in case of '*' next
            i_pat += 1
            i_txt += 1
        elif pattern[i_pat] == '*':
            if prv_ch != '?':
                while i_txt < len(text) and text[i_txt] == prv_ch:
                    i_txt += 1
                i_pat += 1
            else:
                # end '?*' on next character match
                # if no next character, return true (right?)
                if i_pat + 1 == len(pattern):
                    return True

                # find next char in pattern
                i_pat += 1
                assert pattern[i_pat] != '?', "do not handle ?*? yet"

                while i_txt < len(text) and text[i_txt] != pattern[i_pat]:
                    i_txt += 1

    # if not at end of pattern fail (except if '*')
    if i_pat != len(pattern) and not (i_pat == len(pattern) - 1 and pattern[i_pat] == '*'):
        return False

    return True


s = "hello"


assert(pattern_match(s,"hello") is True)
assert(pattern_match(s,"h3ll0") is False)
assert(pattern_match(s,"h?llo") is True)
assert(pattern_match(s,"?????") is True)
assert(pattern_match(s,"h??lo") is True)
assert(pattern_match(s,"h?el?") is False)
assert(pattern_match(s,"hel*?") is True)
assert(pattern_match(s,"hel*o") is True)
assert(pattern_match(s,"he*o") is False)
assert(pattern_match(s,"hel*o*") is True)
assert(pattern_match(s,"h?*") is True)
assert(pattern_match(s,"h?*o") is True)
assert(pattern_match(s,"h?*llo") is True)
assert(pattern_match(s,"?*o") is True)
assert(pattern_match(s,"hel?*f") is False)

