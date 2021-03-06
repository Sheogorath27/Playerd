# encoding: utf-8

__name__ = 'playerd'
__date__ = 'unreleased'
__version__ = '0.1'
__credits__ = '''
    Copyright (C) 2014,2015 Copyleft Games Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

import os

top = '.'
out = 'build'
platforms = ["systemd", 'android', 'mac', 'windows'] #list of available platforms

def options(opt) :
    opt.load('compiler_c')
    opt.add_option("--platform", action='store',
                   default=False, help='Select your playerd platform. [systemd, android, mac, windows]')
    opt.add_option('--android-ndk', action='store',
                   default=os.getenv('ANDROID_NDK'), help='Android NDK directory.')
    opt.add_option('--android-sdk', action='store',
                   default=os.getenv('ANDROID_HOME'), help='Android SDK directory.')
    opt.add_option('--android-arch', action='store',
                   default='armv6', help='Android arcitecture. (armv6, armv7, x86)')
    opt.add_option('--android-level', action='store',
                   default='10', help='Android API level.')
    opt.add_option('--android-toolchain', action='store',
                   default=None, help='Path to standalone Android NDK toolchain.')


def configure(conf) :
    if conf.options.platform == 'android' :
        # recurse first to find a cross-compiler
        conf.recurse('android')
    # Check for a C compiler
    conf.load('compiler_c')

    # Write configuration to disk
    conf.define('PLAYERD_NAME', __name__)
    conf.define('PLAYERD_DATE', __date__)
    conf.define('PLAYERD_VERSION', __version__)
    conf.define('PLAYERD_LICENSE', __credits__.replace('\n', '\\n')) # waf bug
    conf.write_config_header ('config.h')

    conf.recurse('src')

    # Check OS
    import platform
    platform_type = ''
    if platform.system().lower().startswith(('linux', 'gnu/linux')): 
        platform_type = 'linux'
    conf.env['PLATFORM_TYPE'] = platform_type

    if platform_type == 'linux':
        conf.recurse('src/linux')

    platform_detect = 'systemd' #default option
    if platform.platform().lower().startswith('windows'):
        platform_detect = 'windows'
    elif platform.platform().lower().startswith('darwin'):
        platform_detect = 'mac'

    if not(conf.options.platform):
        conf.env['PLATFORM'] = platform_detect
    else :
        conf.env['PLATFORM'] = conf.options.platform

    if(conf.env['PLATFORM'] not in platforms):
        conf.fatal("%s is not a valid platform. Available platforms: %s" %(conf.env['PLATFORM'], platforms))
    print("Selected platform: " + conf.env['PLATFORM'])


def build(bld):
    # Defines for .pc
    bld.define('PACKAGE', __name__)
    bld.define('VERSION', __version__)

    bld.recurse('src')
    if bld.env['PLATFORM_TYPE'] == 'linux' and bld.env['PLATFORM'] != 'android':
        bld.recurse('src/linux')

    if bld.env['PLATFORM'] == 'systemd' :
        bld.recurse('systemd')

    if bld.env['PLATFORM'] == 'android' :
        bld.recurse('android')

    if bld.env['PLATFORM'] == 'mac' :
        bld.recurse('mac')

    if bld.env['PLATFORM'] == 'windows' :
        bld.recurse('windows')

def apk(ctx) :
    'builds an apk for deployment on android devices.'
    if (ctx.env['PLATFORM'] == 'android') :
        ctx.recurse('android')
    else :
        return

from waflib.Build import BuildContext
class Apk(BuildContext) :
    cmd = 'apk'
    fun = 'apk'
