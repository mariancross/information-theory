#
# Copyright (C) 2020 Maria Santamaria
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

import argparse
import subprocess
from sys import exit


import matplotlib.pyplot as plt
import numpy as np


def run_program(exec_path, file_path, n):
    proc = subprocess.Popen([exec_path, file_path, str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, _ = proc.communicate()
    stdout = stdout.decode('utf-8')
    return stdout


def parse_output(stdout):
    data_ = stdout.split()
    entropy = float(data_[1])
    size = float(data_[4])
    return entropy, size


def plot_data(x, y, x_label, y_label, filename):
    plt.plot(x, y)
    plt.xticks(x)
    plt.xlabel(x_label, fontsize='medium')
    plt.ylabel(y_label, fontsize='medium')
    plt.savefig(filename, bbox_inches='tight')
    plt.show()
    plt.cla()
    plt.clf()
    plt.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--exec_path', type=str, required=True)
    parser.add_argument('--file_path', type=str, required=True)
    args = parser.parse_args()

    entropy_l = []
    size_l = []

    for i in range(2, 20):
        stdout = run_program(args.exec_path, args.file_path, i)
        entropy, size = parse_output(stdout)
        entropy_l.append(entropy)
        size_l.append(size)

    entropy_l = np.array(entropy_l)
    size_l = np.array(size_l)

    plot_data(range(2, 20), entropy_l, 'n', 'Entropy', 'entropy.png')
    plot_data(range(2, 20), size_l, 'n', 'Compressed size', 'size.png')
    exit()
