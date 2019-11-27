#include "OBJparser.h"

ObjParser::ObjParser()
{

}
ObjParser::~ObjParser()
{

}

void ObjParser::Parse(const char* input)
{
	strcpy_s(filename, input);

	GetModelFilename();
	ReadFileCounts();
	LoadDataStructures();
}

void ObjParser::GetModelFilename()
{
	bool done;
	ifstream fin;

	// Loop until we have a file name.
	done = false;

	while (!done)
	{
		// Attempt to open the file.
		fin.open(filename);

		if (fin.good())
		{
			// If the file exists and there are no problems then exit since we have the file name.
			done = true;
		}

		else
		{
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
		}
	}

	return;
}

bool ObjParser::ReadFileCounts()
{
	ifstream fin;
	char input = NULL;

	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);

	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}

bool ObjParser::LoadDataStructures()
{
	ifstream fin;
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input = NULL, input2 = NULL;
	ofstream fout;

	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	dVertices = new double[vertexCount * 3];

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;

				dVertices[0 + (vertexIndex * 3)] = (double)vertices[vertexIndex].x;
				dVertices[1 + (vertexIndex * 3)] = (double)vertices[vertexIndex].y;
				dVertices[2 + (vertexIndex * 3)] = (double)vertices[vertexIndex].z;

				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{

			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >>
					faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3 >>
					faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;

				dIndices.push_back(faces[faceIndex].vIndex1 - 1);
				dIndices.push_back(faces[faceIndex].vIndex2 - 1);
				dIndices.push_back(faces[faceIndex].vIndex3 - 1);

				faceIndex++;
			}
		}


		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.fin.close();
	// Open the output file.
	fout.open("model.txt");

	// Write out the file header that our model format uses.
	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;
		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;
		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

	}

	// Close the output file.
	fout.close();

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}