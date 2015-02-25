/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) 2015 Zaubermeister <derzauberer77@ymail.com>
 * 
 * pdfcomp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * pdfcomp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>

void extractMetadata();

int main(int argc, char* argv[])
{
	if(getuid() != 0){
		std::cout << "You must be root to properly use this command. " << "Even though you must use sudo, you, not root (unless you are logged in as root), will be the owner (and the group) of the new file." << std::endl;
		return 0;}

	std::string rightWay = "\nUsage: To greatly compress a PDF file, enter the -s switch, followed by the source PDF, the -e switch if you ";
			rightWay+="wish to edit the PDF metadata, and the -o switch followed by the name of the desired output file.\n \n";
			rightWay+="Example: \"pdfcomp -s ImBloated.pdf -e -o ImSlimmerNow.pdf\"\n\nThe following also works: \"pdfcomp --source ImBloated.pdf ";
			rightWay+="--edit-metadata --output ImSlimmerNow.pdf\"\n\n";
			/*rightWay+="(optional) Enter -d followed by the desired document resolution (an integer; currently 150 dpi)\n\n";*/


	if(argc < 2)
	{
		std::cerr << rightWay /*<<
			"Use the -h switch after calling the command to " <<
			"see a much more full list of commands."*/;

		return 0;
	}



	std::string filename = "";
	std::string outfile = "";
	bool edit = 0;
	std::string res = "150 ";

	
	for (int i = 1; i<argc; i++)
	{
		
		std::string arg = argv[i];
		if(arg == "-s" || arg == "--source")
			filename+=argv[i+1];

		if(arg == "-o" || arg == "--output")
			outfile+=argv[i+1];
		if(arg == "-e" || arg == "--edit-metadata")
			edit = 1;
		/*if(arg == "-d" || arg == "--document-resolution")
			res+=argv[i+1];*/

	}

	/*if(res.length() == 0)
		res == "150";*/
	
	system("rm -r /tmp/pdfcomp");
	system("mkdir /tmp/pdfcomp");

	system("cp /usr/lib/pdfcomp/pdfcompScript /tmp/pdfcomp/script");
	std::string metaCommand = "/opt/pdftk-newer ";
	metaCommand+=filename;
	metaCommand+= " dump_data >> /tmp/pdfcomp/datadump.txt";

	system(metaCommand.c_str());
	
	if (edit == 1)
	{
		std::cout << "\nPDFCOMP will open the metadata with GNU Nano.\n";

		system("nano /tmp/pdfcomp/datadump.txt");

		std::cout << "Done editing metadata with Nano." << std::endl;

	}

	//Input Script Values
	std::string scriptreplace = "sed -i 's/TheInputFile/";
	scriptreplace+=filename;
	scriptreplace+="/' /tmp/pdfcomp/script";

	std::string scriptreplace2 = "sed -i 's/TheOutputFile/";
	scriptreplace2+=outfile;
	scriptreplace2+="/' /tmp/pdfcomp/script";

	system(scriptreplace.c_str());
	system(scriptreplace2.c_str());

	system("chmod +x /tmp/pdfcomp/script");
	system("/tmp/pdfcomp/script");


	std::string metaUpdate = "/opt/pdftk-newer /tmp/pdfcomp/";
	metaUpdate+=outfile;
	metaUpdate+= " update_info /tmp/pdfcomp/datadump.txt output ";
	metaUpdate+=outfile;

	system(metaUpdate.c_str());

	std::string name = getlogin();

	std::string changeowner = "chown ";
	changeowner+=name;
	changeowner+=":";
	changeowner+=name;
	changeowner+=" ";
	changeowner+=outfile;

	system(changeowner.c_str());

	return 0;
}