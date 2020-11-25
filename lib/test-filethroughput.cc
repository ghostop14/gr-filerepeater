/* -*- c++ -*- */
/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <iomanip>
#include <string.h>
#include <fstream>
#include <chrono>
#include <ctime>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef O_BINARY
#define	OUR_O_BINARY O_BINARY
#else
#define	OUR_O_BINARY 0
#endif

#ifdef O_LARGEFILE
#define	OUR_O_LARGEFILE	O_LARGEFILE
#else
#define	OUR_O_LARGEFILE 0
#endif

std::string test_file = "/tmp/test_file.bin";
long num_items = 16384;
int iterations = 200;

class comma_numpunct : public std::numpunct<char>
{
  protected:
    virtual char do_thousands_sep() const
    {
        return ',';
    }

    virtual std::string do_grouping() const
    {
        return "\03";
    }
};


// Doubles were chosen here to simulate float complex (both being 8 bytes wide).
size_t bufferToFile(FILE *d_fp, const char *filename, double *buffer, long long unsigned int length) {
	// Write the data
    char *inbuf = (char*)buffer;
    size_t nwritten = 0;
    int d_itemsize = sizeof(double);

    while(nwritten < length) {
    	// fwrite: returns number of elements written
    	// Takes: ptr to array of elements, element size, count, file stream pointer
      long count = fwrite(inbuf, d_itemsize, length - nwritten, d_fp);
      if(count == 0) {
    	  // Error condition, nothing written for some reason.
        if(ferror(d_fp)) {
          printf("file write error.\n");
          break;
        }
        else { // is EOF?  Probably will never get to this break;
          break;
        }
      }
      nwritten += count;
      inbuf += count * d_itemsize;
    }

	return nwritten;
}

bool testWrite() {
	std::cout << "----------------------------------------------------------" << std::endl;
	std::cout << "Testing File write throughput" << std::endl;

	// try opening the file
	FILE *d_fp = NULL;
	int fd;
	int flags;
	flags = O_WRONLY|O_CREAT|O_TRUNC|OUR_O_LARGEFILE|OUR_O_BINARY;
	if((fd = open(test_file.c_str(), flags, 0664)) < 0){
		std::cout << "ERROR: Unable to open file " << test_file << std::endl;
		return false;
	}

	if((d_fp = fdopen (fd, "wb")) == NULL) {
		std::cout << "ERROR: Unable to open file " << test_file << " for writing." << std::endl;
		fclose(d_fp);        // don't leak file descriptor if fdopen fails.
		return false;
	}

	int i;
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<double> elapsed_seconds = end-start;


	// Run empty test
	float elapsed_time,throughput;
	long input_buffer_total_bytes;
	float bits_throughput;

	double *test_buffer;
	test_buffer = new double[num_items];

	long bytes_written = 0;

	start = std::chrono::steady_clock::now();

	for (int i=0;i<iterations;i++) {
		bytes_written += bufferToFile(d_fp, test_file.c_str(), test_buffer, num_items);
	}
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end-start;

	elapsed_time = elapsed_seconds.count();

	throughput = bytes_written / elapsed_time;
	bits_throughput = 8 * bytes_written / elapsed_time;

	std::cout << std::endl << "File write performance:" << std::endl;
	std::cout << "Wrote " << bytes_written << " total bytes to file " << test_file << std::endl;
	std::cout << "Elapsed time: " << elapsed_seconds.count() << std::endl;
	std::cout << "Timing Averaging Iterations: " << iterations << std::endl;
	std::cout << "Per iteration write size: " << num_items * sizeof(double) << " bytes" << std::endl;
	std::cout << "Average Run Time:   " << std::fixed << std::setw(11) << std::setprecision(6) << elapsed_time << " s" << std::endl <<
				"Byte Throughput: " << std::setprecision(2) << throughput << " Bytes/sec" << std::endl <<
				"Byte IQ Throughput: " << std::setprecision(2) << throughput/2 << " Byte IQ (IChar)/sec" << std::endl <<
				"Complex throughput: " << std::setprecision(2) << throughput / 8 << " float complex samples/sec" << std::endl <<
				"Projected processing rate: " << bits_throughput << " bps" << std::endl;

	delete[] test_buffer;

	// Clean up
	fclose(d_fp);

	return true;
}

int
main (int argc, char **argv)
{
	// Add comma's to numbers
	std::locale comma_locale(std::locale(), new comma_numpunct());

	// tell cout to use our new locale.
	std::cout.imbue(comma_locale);

	if (argc > 1) {
		// 1 is the file name
		if (strcmp(argv[1],"--help")==0) {
			std::cout << std::endl;
			std::cout << "Usage: test-filethroughput [<test file name>]" << std::endl;
			std::cout << "Where the default file is /tmp/test_file.bin" << std::endl;
			std::cout << std::endl;
			exit(0);
		}

		test_file = argv[1];
	}

	bool was_successful;

	was_successful = testWrite();

	std::cout << std::endl;

	return 0;
}

