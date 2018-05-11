//Paul Tudan and Jefferson Kappers
//5-11-18

class Controler {
	void readInputFile() {
        string testOutputSting
        ofstream inputFile;
        inputFile.open('input.txt');
        if (inputFile.is_open()) {
            while (!inputFile.eof) {
                getline(inputFile,oneLineString);
                cout<<oneLineString;
                for (i = 0; i <= oneLineString.size(); i++) {
                    if (oneLineString[i] == ) {

                    }
                }
            }
        }
        else {
            cout << "Unable to open file";
        }
    }	
}