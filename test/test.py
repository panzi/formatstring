#!/usr/bin/env python

import sys
import re
from subprocess import Popen, PIPE
from ctypes import c_uint8

STR_REPR = {
	'\0': '\\0',
	'\a': '\\a',
	'\b': '\\b',
	'\t': '\\t',
	'\n': '\\n',
	'\v': '\\v',
	'\f': '\\f',
	'\r': '\\r',
	'"':  '\\"',
	'?':  '\\?',
	'\\': '\\\\'
}

def crepr(val):
	t = type(val)
	if t in (int, long):
		if t < -0x7fffffff or t > 0x7fffffff:
			return str(val)+'L'
		else:
			return str(val)
	elif t is float:
		return str(val)
	elif t is str:
		return cstr(val)
	elif t is bool:
		if val:
			return 'true'
		else:
			return 'false'
	else:
		raise TypeError, "unsupported type: "+t

def cstr(s):
	buf = ['"']
	for c in s:
		buf.append(STR_REPR.get(c,c))
	buf.append('"')
	return ''.join(buf)

def comb(xs,ys):
	for x in xs:
		for y in ys:
			yield x + y

def combs(xs,*rest):
	if rest:
		ys, rest = rest[0], rest[1:]
		if rest:
			return combs(comb(xs,ys), *rest)
		else:
			return comb(xs,ys)
	else:
		return xs

bool_values  = [True, False]
str_values   = ['', 'a', ' Foo\nBar\tBaz! ']

int8_values  = [-0x7f, -23, -5, -1, 0, 1, 5, 23, 65, 0x7f]
int16_values = [-0x7fff, -1000, -100] + int8_values + [100, 1000, 0x7fff]
int32_values = [-0x7fffffff] + int16_values + [0x7fffffff]
int64_values = [-0x7fffffffffffffff] + int32_values + [0x7fffffffffffffff]

uint8_values  = [0, 1, 5, 23, 65, 0xff]
uint16_values = uint8_values + [100, 1000, 0xffff]
uint32_values = uint16_values + [0xffffffff]
uint64_values = uint32_values + [0xffffffffffffffff]

float_values = [
	-1000000015047466219876688855040.000000,
	-1234.56789, -1000.0, -100.0, -1.0, -0.0, 0.0, 1.0, 100.0, 1000.0, 1234.56789,
	1000000015047466219876688855040.000000,
	float('nan'), float('-inf'), float('inf')]

convs  = ['', '!r', '!s']
aligns = ['', '_<', '_>', '_=', '_^']
signs  = ['', '+', '-', ' ']
alts   = ['', '#']
widths = ['', '0', '01', '016']
tdssep = ['', ',']
precs  = ['', '.0', '.1', '.6', '.12']

str_aligns = ['_<', '_>', '_^']

# TODO: Python supports less types than the C++ formatstring libraray
#char_types  = ['c']
#int_types   = ['b', 'B', 'd', 'o', 'O', 'x', 'X']
#float_types = ['e', 'E', 'f', 'F', 'g', 'G', '%', 'a', 'A']
#str_types   = ['s', 'S']
char_types   = []
nondec_types = ['b', 'o', 'x']
int_types    = nondec_types + ['d']
float_types  = ['e', 'E', 'f', 'F', 'g', 'G', '%']
str_types    = ['s']
all_types    = char_types + int_types + float_types + str_types

bool_specs = \
	list(comb(convs, (':'+spec for spec in
		combs(aligns, widths, [''] + char_types + int_types + float_types))))

int_specs = \
	list(comb(convs, (':'+spec for spec in
		combs(aligns, signs, alts, widths, [''] + nondec_types)))) + \
	list(comb(convs, (':'+spec for spec in
		combs(aligns, signs, alts, widths, tdssep, ['', 'd'])))) + \
	list(comb(convs, (':'+spec for spec in
		combs(aligns, signs, widths, tdssep, [''] + float_types))))

char_specs = \
	list(comb(convs, (':'+spec for spec in
		combs(aligns, signs, alts, widths, [''] + nondec_types)))) + \
	list(comb(convs, (':'+spec for spec in
		combs(aligns, signs, alts, widths, tdssep, ['', 'd'])))) + \
	list(comb(convs, (':'+spec for spec in
		combs(aligns, signs, widths, tdssep, [''] + float_types))))

float_specs = \
	list(comb(convs, (':'+spec for spec in
		combs(aligns, signs, widths, tdssep, [''] + float_types))))

str_specs = \
	list(comb(convs, (':'+spec for spec in
		combs(str_aligns, widths, [''] + str_types)))) + \
	list(comb(convs, (':'+spec for spec in [''] + str_types)))

basic_formats = ['{}', '{!r}', '{!s}', '{0}', 'foo {0} bar {0} baz']

bool_formats  = basic_formats + ['{%s}' % spec for spec in bool_specs]
char_formats  = basic_formats + ['{%s}' % spec for spec in char_specs]
int_formats   = basic_formats + ['{%s}' % spec for spec in int_specs]
float_formats = basic_formats + ['{%s}' % spec for spec in float_specs]
str_formats   = basic_formats + ['{%s}' % spec for spec in str_specs]

testcases = [
#	('format_bool', 'bool', bool_values, bool_formats),

	('format_char', 'char', int8_values, char_formats),

	('format_int8', 'std::int8_t', int8_values, int_formats),
	('format_int16', 'std::int16_t', int16_values, int_formats),
	('format_int32', 'std::int32_t', int32_values, int_formats),
	('format_int64', 'std::int64_t', int64_values, int_formats),
	
	('format_uint8', 'std::uint8_t', uint8_values, int_formats),
	('format_uint16', 'std::uint16_t', uint16_values, int_formats),
	('format_uint32', 'std::uint32_t', uint32_values, int_formats),
	('format_uint64', 'std::uint64_t', uint64_values, int_formats),

	('format_float', 'float', float_values, float_formats),
	('format_double', 'double', float_values, float_formats),
	('format_long_double', 'long double', float_values, float_formats),

	('format_str', None, str_values, str_formats)
]

def generate(out):
	out.write("""\
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <cstdint>

#include <formatstring.h>

using namespace formatstring;

""")

	for name, tp, values, formats in testcases:
		if tp:
			tp = '(%s)' % tp
		else:
			tp = ''

		out.write("""
BOOST_AUTO_TEST_CASE({name}) {{
	std::string s;

""".format(name=name))

		for fmt in formats:
			cfmt = cstr(fmt)
			out.write('\t/* {format} */\n'.format(format=cfmt))
			for val in values:
				out.write("""\
	s = format({fmt}, {cast}{val});
	BOOST_CHECK_EQUAL(s, {str});
""".format(fmt=cfmt, cast=cast, val=crepr(val), str=crepr(fmt.format(val))))
			out.write('\n')

		out.write('}\n')

def run_test(binary,tp,fmt,value):
	pyres = fmt.format(value)
	if tp == 'char' or tp == 'unsigned char':
		if type(value) is str:
			svalue = value
		elif value < 0:
			svalue = chr(c_uint8(-value).value)
		else:
			svalue = chr(value)
	else:
		svalue = str(value)
	pipe = Popen([binary, fmt, tp, svalue], stdout=PIPE, stderr=PIPE)
	if pipe.wait() == 0:
		cppres = pipe.stdout.read()
		if pyres == cppres:
			sys.stdout.write("[  OK  ] %r: %r == %r\n" % (value, pyres, cppres))
		else:
			sys.stdout.write("[ FAIL ] %r: %r != %r\n" % (value, pyres, cppres))
			sys.exit(1)
	else:
		error = pipe.stderr.read()
		sys.stdout.write("[ FAIL ] %r: %s\n" % (value, error))
		sys.exit(1)

def run_tests(binary):
	for name, tp, values, formats in testcases:
#		sys.stdout.write("TEST: %s\n" % name)
		for fmt in formats:
			sys.stdout.write("TEST: %s %r %s\n" % (name, fmt, tp))
			for value in values:
				run_test(binary,tp,fmt,value)
		sys.stdout.write("\n")

if __name__ == '__main__':
	run_tests(sys.argv[1])
