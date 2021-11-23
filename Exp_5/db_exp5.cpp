#include<bits/stdc++.h>
#include"extmem.h"

using namespace std;
int cmpR(const void* a, const void* b);
int cmpS(const void* a, const void* b);
int LinearSearch();
int TPMMS_R_binary_merge();
int TPMMS_S_binary_merge();
int Bplus_index_search();
int sort_merge_join();
int R_union_S();
struct nodeR
{
	int A;
	int B;
}R[56];
struct nodeS
{
	int C;
	int D;
}S[112];
int main()
{
	cout << "==========Start LinerSearch==========" << endl;
	int LS = LinearSearch();
	if (!LS)
		cout << "LinearSearch success" << endl;

	cout << "==========Start TPMMS==========" << endl;
	int TR = TPMMS_R_binary_merge();
	if (!TR)
		cout << "TPMMS_R_binary_merge success" << endl;
	int TS = TPMMS_S_binary_merge();
	if (!TS)
		cout << "TPMMS_S_binary_merge success" << endl;

	cout << "==========Start B+ tree index search==========" << endl;
	int B = Bplus_index_search();
	if (!B)
		cout << "Bplus_index_search success" << endl;

	cout << "==========Start Sort-Merge-Join==========" << endl;
	int SMJ = sort_merge_join();
	if (!SMJ)
		cout << "Sort-Merge-Join success" << endl;

	cout << "==========R_union_S==========" << endl;
	int RUS = R_union_S();
	if (!RUS)
		cout << "R_union_S success" << endl;
}
int cmpR(const void* a, const void* b)
{
	struct nodeR* c = (nodeR*)a;
	struct nodeR* d = (nodeR*)b;
	if (c->A != d->A)return c->A - d->A;
	else return c->B - d->B;
}
int cmpS(const void* a, const void* b)
{
	struct nodeS* c = (nodeS*)a;
	struct nodeS* d = (nodeS*)b;
	if (c->C != d->C)return c->C - d->C;
	else return c->D - d->D;
}
//S.C=130//select S.C,S.D from S where S.C=130//answer sorted in 49.blk
int LinearSearch()
{
	Buffer buf;
	unsigned char* blk, * result_blk;
	int result_length = 0;
	int result_blk_addr = 49;
	int tmp_C, tmp_D;
	int next_blk = 17;
	char str[5] = { '\0' };
	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	result_blk = getNewBlockInBuffer(&buf);
	memset(result_blk, 0, sizeof(result_blk));
	cout << "LinerSearch:select S.C,S.D from S where S.C=130" << endl;
	cout << "S.C" << "\t" << "S.D" << endl;
	while (next_blk != 49)
	{
		if ((blk = readBlockFromDisk(next_blk, &buf)) == NULL)
		{
			perror("Reading Block Failed!\n");
			return -1;
		}
		for (int i = 0; i < 7; i++)
		{
			//Get C
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j);
			tmp_C = atoi(str);
			//Get D
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j + 4);
			tmp_D = atoi(str);
			//Find C=130
			if (tmp_C == 130)
			{
				sprintf(str, "%d", tmp_C);
				for (int j = 0; j < strlen(str); j++)
					*(result_blk + result_length + j) = str[j];
				sprintf(str, "%d", tmp_D);
				for (int j = 0; j < strlen(str); j++)
					*(result_blk + result_length + j + 4) = str[j];
				result_length += 8;
				cout << tmp_C << "\t" << tmp_D << endl;
			}
		}
		//if current result-block is full,restore answer in next block 
		if (result_length == 56) 
		{
			sprintf(str, "%d", result_blk_addr + 1);
			for (int j = 0; j < strlen(str); j++) 
				*(result_blk + 56 + j) = str[j];
			if (writeBlockToDisk(result_blk, result_blk_addr, &buf) != 0) 
			{
				perror("Writing Block Failed!\n");
				return -1;
			}
			freeBlockInBuffer(result_blk, &buf);
			result_blk = getNewBlockInBuffer(&buf);
			memset(result_blk, 0, 64);
			result_length = 0;
			result_blk_addr++;
		}
		for (int j = 0; j < 4; j++)
			str[j] = *(blk + 56 + j);
		next_blk = atoi(str);
		freeBlockInBuffer(blk, &buf);
	}
	sprintf(str, "%d", 0);
	for (int j = 0; j < strlen(str); j++)
		*(result_blk + 56 + j) = str[j];
	if (writeBlockToDisk(result_blk, result_blk_addr, &buf) != 0)
		perror("Writing Block Failed!\n");
	freeBlockInBuffer(result_blk, &buf);
	cout << "The IO of Searching S.C=130 :" << buf.numIO << endl;
	return 0;
}
//TPMMS for R//ans starts from 1000.blk
int TPMMS_R_binary_merge()
{
	Buffer buf;
	unsigned char* blk;
	int next_blk = 1;
	int cnt = 0;
	int tmp_A, tmp_B;
	int tmp_result_blk_addr = 101;
	memset(R, 0, sizeof(R));
	char str[5] = { '\0' };

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	//qsort for R
	while (next_blk != 17)
	{
		if ((blk = readBlockFromDisk(next_blk, &buf)) == NULL)
		{
			perror("Reading Block Failed!\n");
			return -1;
		}
		//Get elements in R into defined struct R
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j);
			tmp_A = atoi(str);
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j + 4);
			tmp_B = atoi(str);
			R[cnt].A = tmp_A;
			R[cnt++].B = tmp_B;
		}
		for (int j = 0; j < 4; j++)
			str[j] = *(blk + 56 + j);
		next_blk = atoi(str);
		freeBlockInBuffer(blk, &buf);
		//sort all element for each part of R (which has 8 blocks) by ascending order according to R.A and then R.B 
		if (cnt == 56)
		{
			cnt = 0;
			qsort(R, 56, sizeof(R[0]), cmpR);
			blk = getNewBlockInBuffer(&buf);
			memset(blk, 0, sizeof(blk));
			int i = 0;
			//write temp result into disk
			while (i < 56)
			{
				for (int j = 0; j < 7; j++)
				{
					tmp_A = R[i].A;
					tmp_B = R[i].B;
					i++;
					sprintf(str, "%d", tmp_A);
					for (int k = 0; k < strlen(str); k++)
						*(blk + 8 * j + k) = str[k];
					sprintf(str, "%d", tmp_B);
					for (int k = 0; k < strlen(str); k++)
						*(blk + 8 * j + k + 4) = str[k];
				}
				if (i == 56)
					sprintf(str, "%d", 0);
				else
					sprintf(str, "%d", tmp_result_blk_addr + 1);
				for (int k = 0; k < strlen(str); k++)
					*(blk + 56 + k) = str[k];
				if (writeBlockToDisk(blk, tmp_result_blk_addr, &buf) != 0)
				{
					perror("Writing Block Failed!\n");
					return -1;
				}
				tmp_result_blk_addr++;
				blk = getNewBlockInBuffer(&buf);
				memset(blk, 0, sizeof(blk));
			}
			tmp_result_blk_addr += 2;
		}
	}

	//merge R
	int blk1_addr = 101;
	int blk2_addr = 111;
	int tmp_A1, tmp_B1, tmp_A2, tmp_B2;
	int result_addr = 1000;
	int result_length = 0;
	unsigned char* blk1, * blk2;
	int i = 0, j = 0;
	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	blk = getNewBlockInBuffer(&buf);
	memset(blk, 0, sizeof(blk));
	blk1 = readBlockFromDisk(blk1_addr, &buf);
	blk2 = readBlockFromDisk(blk2_addr, &buf);
	cout << "R.A" << "\t" << "R.B" << endl;
	while (blk1_addr != 8 || blk2_addr != 18)
	{
		//get A1,A2,B1,B2
		for (int k = 0; k < 4; k++)
			str[k] = *(blk1 + 8 * i + k);
		tmp_A1 = atoi(str);
		for (int k = 0; k < 4; k++)
			str[k] = *(blk1 + 8 * i + k + 4);
		tmp_B1 = atoi(str);
		for (int k = 0; k < 4; k++)
			str[k] = *(blk2 + 8 * j + k);
		tmp_A2 = atoi(str);
		for (int k = 0; k < 4; k++)
			str[k] = *(blk2 + 8 * j + k + 4);
		tmp_B2 = atoi(str);

		//if the first part of R ends ,write the rest of the second part
		if (blk1_addr == 8)
		{
			j++;
			sprintf(str, "%d", tmp_A2);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k) = str[k];
			sprintf(str, "%d", tmp_B2);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k + 4) = str[k];
			result_length += 8;
			cout << tmp_A2 << "\t" << tmp_B2 << endl;
		}

		//if the second part of R ends,write the rest of the first part
		else if (blk2_addr == 18)
		{
			i++;
			sprintf(str, "%d", tmp_A1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k) = str[k];
			sprintf(str, "%d", tmp_B1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k + 4) = str[k];
			result_length += 8;
			cout << tmp_A1 << "\t" << tmp_B1 << endl;
		}
		else
		{
			//if (tmp_A1 <= tmp_A2&&tmp_B1<=tmp_B2)
			if (tmp_A1 <= tmp_A2)
			{
				i++;
				sprintf(str, "%d", tmp_A1);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k) = str[k];
				sprintf(str, "%d", tmp_B1);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k + 4) = str[k];
				result_length += 8;
				cout << tmp_A1 << "\t" << tmp_B1 << endl;
			}
			else
			{
				j++;
				sprintf(str, "%d", tmp_A2);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k) = str[k];
				sprintf(str, "%d", tmp_B2);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k + 4) = str[k];
				result_length += 8;
				cout << tmp_A2 << "\t" << tmp_B2 << endl;
			}
		}
		/*
		if the block is end then read elements from the next block
		*/
		if (i == 7)
		{
			for (int k = 0; k < 4; k++)
				str[k] = *(blk1 + 56 + k);
			blk1_addr = atoi(str);
			i = 0;
			freeBlockInBuffer(blk1, &buf);
			if (blk1_addr != 0)
			{
				if ((blk1 = readBlockFromDisk(blk1_addr, &buf)) == NULL)
				{
					perror("Reading Block Failed\n");
					return -1;
				}
			}
		}
		if (j == 7)
		{
			for (int k = 0; k < 4; k++)
				str[k] = *(blk2 + 56 + k);
			blk2_addr = atoi(str);
			j = 0;
			freeBlockInBuffer(blk2, &buf);
			if (blk2_addr != 0)
			{
				if ((blk2 = readBlockFromDisk(blk2_addr, &buf)) == NULL)
				{
					perror("Reading Block Failed\n");
					return -1;
				}
			}
		}

		//if the result block is full then write it to disk and get a new one
		if (result_length == 56)
		{
			if (blk1_addr == 0 && blk2_addr == 0)
				sprintf(str, "%d", 0);
			else
				sprintf(str, "%d", result_addr + 1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + 56 + k) = str[k];
			if (writeBlockToDisk(blk, result_addr, &buf) != 0)
			{
				perror("Writing Block Failed!\n");
				return -1;
			}
			blk = getNewBlockInBuffer(&buf);
			memset(blk, 0, sizeof(blk));
			result_length = 0;
			result_addr++;
		}
	}
	return 0;
}
//TPMMS for S//ans starts from 2000.blk
int TPMMS_S_binary_merge()
{
	Buffer buf;
	unsigned char* blk;
	int next_blk = 17;
	int cnt = 0;
	int tmp_C, tmp_D;
	int tmp_result_blk_addr = 201;
	memset(S, 0, sizeof(S));
	char str[5] = { '\0' };

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	//qsort for S
	while (next_blk != 49)
	{
		if ((blk = readBlockFromDisk(next_blk, &buf)) == NULL)
		{
			perror("Reading Block Failed!\n");
			return -1;
		}
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j);
			tmp_C = atoi(str);
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j + 4);
			tmp_D = atoi(str);
			S[cnt].C = tmp_C;
			S[cnt++].D = tmp_D;
		}
		for (int j = 0; j < 4; j++)
			str[j] = *(blk + 56 + j);
		next_blk = atoi(str);
		freeBlockInBuffer(blk, &buf);
		if (cnt == 112)
		{
			cnt = 0;
			qsort(S, 112, sizeof(S[0]), cmpS);
			blk = getNewBlockInBuffer(&buf);
			memset(blk, 0, sizeof(blk));
			int i = 0;
			while (i < 112)
			{
				for (int j = 0; j < 7; j++)
				{
					tmp_C = S[i].C;
					tmp_D = S[i].D;
					i++;
					sprintf(str, "%d", tmp_C);
					for (int k = 0; k < strlen(str); k++)
						*(blk + 8 * j + k) = str[k];
					sprintf(str, "%d", tmp_D);
					for (int k = 0; k < strlen(str); k++)
						*(blk + 8 * j + k + 4) = str[k];
				}
				if (i == 112)
					sprintf(str, "%d", 0);
				else
					sprintf(str, "%d", tmp_result_blk_addr + 1);
				for (int k = 0; k < strlen(str); k++)
					*(blk + 56 + k) = str[k];
				if (writeBlockToDisk(blk, tmp_result_blk_addr, &buf) != 0)
				{
					perror("Writing Block Failed!\n");
					return -1;
				}
				tmp_result_blk_addr++;
				blk = getNewBlockInBuffer(&buf);
				memset(blk, 0, sizeof(blk));
			}
			tmp_result_blk_addr += 2;
		}
	}
	//merge for S
	int blk1_addr = 201;
	int blk2_addr = 219;
	int tmp_C1, tmp_D1, tmp_C2, tmp_D2;
	int result_addr = 2000;
	int result_length = 0;
	unsigned char* blk1, * blk2;
	int i = 0, j = 0;
	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	blk = getNewBlockInBuffer(&buf);
	memset(blk, 0, sizeof(blk));
	blk1 = readBlockFromDisk(blk1_addr, &buf);
	blk2 = readBlockFromDisk(blk2_addr, &buf);
	cout << "S.C" << "\t" << "S.D" << endl;
	while (blk1_addr != 16 || blk2_addr != 34)
	{
		for (int k = 0; k < 4; k++)
			str[k] = *(blk1 + 8 * i + k);
		tmp_C1 = atoi(str);
		for (int k = 0; k < 4; k++)
			str[k] = *(blk1 + 8 * i + k + 4);
		tmp_D1 = atoi(str);


		for (int k = 0; k < 4; k++)
			str[k] = *(blk2 + 8 * j + k);
		tmp_C2 = atoi(str);
		for (int k = 0; k < 4; k++)
			str[k] = *(blk2 + 8 * j + k + 4);
		tmp_D2 = atoi(str);

		if (blk1_addr == 16)
		{
			j++;
			sprintf(str, "%d", tmp_C2);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k) = str[k];
			sprintf(str, "%d", tmp_D2);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k + 4) = str[k];
			result_length += 8;
			cout << tmp_C2 << "\t" << tmp_D2 << endl;
		}
		else if (blk2_addr == 34)
		{
			i++;
			sprintf(str, "%d", tmp_C1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k) = str[k];
			sprintf(str, "%d", tmp_D1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k + 4) = str[k];
			result_length += 8;
			cout << tmp_C1 << "\t" << tmp_D1 << endl;
		}
		else
		{
			//if (tmp_C1 <= tmp_C2&&tmp_D1<=tmp_D2)
			if (tmp_C1 <= tmp_C2)
			{
				i++;
				sprintf(str, "%d", tmp_C1);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k) = str[k];
				sprintf(str, "%d", tmp_D1);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k + 4) = str[k];
				result_length += 8;
				cout << tmp_C1 << "\t" << tmp_D1 << endl;
			}
			else
			{
				j++;
				sprintf(str, "%d", tmp_C2);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k) = str[k];
				sprintf(str, "%d", tmp_D2);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k + 4) = str[k];
				result_length += 8;
				cout << tmp_C2 << "\t" << tmp_D2 << endl;
			}
		}

		if (i == 7)
		{
			for (int k = 0; k < 4; k++)
				str[k] = *(blk1 + 56 + k);
			blk1_addr = atoi(str);
			i = 0;
			freeBlockInBuffer(blk1, &buf);
			if (blk1_addr != 0)
			{
				if ((blk1 = readBlockFromDisk(blk1_addr, &buf)) == NULL)
				{
					perror("Reading Block Failed\n");
					return -1;
				}
			}
		}
		if (j == 7)
		{
			for (int k = 0; k < 4; k++)
				str[k] = *(blk2 + 56 + k);
			blk2_addr = atoi(str);
			j = 0;
			freeBlockInBuffer(blk2, &buf);
			if (blk2_addr != 0)
			{
				if ((blk2 = readBlockFromDisk(blk2_addr, &buf)) == NULL)
				{
					perror("Reading Block Failed\n");
					return -1;
				}
			}
		}
		if (result_length == 56)
		{
			if (blk1_addr == 0 && blk2_addr == 0)
				sprintf(str, "%d", 0);
			else
				sprintf(str, "%d", result_addr + 1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + 56 + k) = str[k];

			if (writeBlockToDisk(blk, result_addr, &buf) != 0)
			{
				perror("Writing Block Failed!\n");
				return -1;
			}
			blk = getNewBlockInBuffer(&buf);
			memset(blk, 0, sizeof(blk));
			result_length = 0;
			result_addr++;
		}
	}
	return 0;
}
//B+ index select for S //select S.C,S.D from S where S.C=130//B+ tree sorted from 3000.blk//result sorted from 300.blk
int Bplus_index_search()
{
	Buffer buf;
	unsigned char* blk;
	int tmp_C, tmp_D;
	char str[5] = { '\0' };
	int S_next = 2000;
	unsigned char* leaf;
	unsigned char* blk1;
	int leaf_length = 0;
	int blk1_length = 0;
	int leaf_addr = 3000;
	int blk1_addr = 4000;
	int max = 0;

	unsigned char* result_blk;
	int result_length = 0;
	int result_addr = 300;
	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	leaf = getNewBlockInBuffer(&buf);
	blk1 = getNewBlockInBuffer(&buf);
	memset(leaf, 0, sizeof(leaf));
	memset(blk1, 0, sizeof(blk1));
	//build B+ tree
	/*
	the maximun of index per node is four
	a index is index number+the first shown blok number
	if the leaf node is full and a new element comes in ,a mid node or root shall break 
	*/
	while (S_next != 2032)
	{
		if ((blk = readBlockFromDisk(S_next, &buf)) == NULL)
		{
			perror("Reading Block Failed!\n");
			return -1;
		}
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j);
			tmp_C = atoi(str);
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j + 4);
			tmp_D = atoi(str);

			//if tmp_C>max then put it into  the tree
			if (tmp_C > max)
			{
				max = tmp_C;
				//judge if the leaf is full
				if (leaf_length < 48)
				{
					sprintf(str, "%d", tmp_C);
					for (int j = 0; j < strlen(str); j++)
						*(leaf + leaf_length + j) = str[j];
					sprintf(str, "%d", S_next);
					for (int j = 0; j < strlen(str); j++)
						*(leaf + leaf_length + j + 4) = str[j];
					leaf_length += 8;
				}

				/*
				if the leaf is full then write it into disk and build a new leaf node
				put the element into new leaf node 
				*/
				else
				{
					sprintf(str, "%d", leaf_addr + 1);
					for (int j = 0; j < strlen(str); j++)
						*(leaf + 56 + j) = str[j];
					if (writeBlockToDisk(leaf, leaf_addr, &buf) != 0)
					{
						perror("Writing Block Failed!\n");
						return -1;
					}
					//get a new leaf node
					leaf = getNewBlockInBuffer(&buf);
					memset(leaf, 0, sizeof(leaf));
					leaf_length = 0;
					leaf_addr++;

					//put the element into leaf
					sprintf(str, "%d", tmp_C);
					for (int j = 0; j < strlen(str); j++)
						*(leaf + leaf_length + j) = str[j];
					sprintf(str, "%d", S_next);
					for (int j = 0; j < strlen(str); j++)
						*(leaf + leaf_length + j + 4) = str[j];
					leaf_length += 8;

					//refresh mid node
					if (blk1_length == 0)
					{
						sprintf(str, "%d", leaf_addr - 1);
						for (int j = 0; j < strlen(str); j++)
							*(blk1 + j) = str[j];
						sprintf(str, "%d", tmp_C);
						for (int j = 0; j < strlen(str); j++)
							*(blk1 + j + 4) = str[j];
						sprintf(str, "%d", leaf_addr);
						for (int j = 0; j < strlen(str); j++)
							*(blk1 + 8 + j) = str[j];
						blk1_length += 12;
					}
					else
						sprintf(str, "%d", tmp_C);
					for (int j = 0; j < strlen(str); j++)
						*(blk1 + blk1_length + j) = str[j];
					sprintf(str, "%d", leaf_addr);
					for (int j = 0; j < strlen(str); j++)
						*(blk1 + blk1_length + j + 4) = str[j];
					blk1_length += 8;
				}
			}
		}
		for (int i = 0; i < 4; i++)
			str[i] = *(blk + 56 + i);
		S_next = atoi(str);
		if (S_next == 16)
			S_next == 219;
		freeBlockInBuffer(blk, &buf);
	}
	sprintf(str, "%d", 0);
	for (int j = 0; j < strlen(str); j++)
		*(leaf + 56 + j) = str[j];
	if (writeBlockToDisk(leaf, leaf_addr, &buf) != 0)
	{
		perror("Writing Block Failed!\n");
		return -1;
	}
	for (int j = 0; j < strlen(str); j++)
		*(blk1 + 56 + j) = str[j];
	if (writeBlockToDisk(blk1, blk1_addr, &buf) != 0)
	{
		perror("Writing Block Failed!\n");
		return -1;
	}
	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	result_blk = getNewBlockInBuffer(&buf);
	memset(result_blk, 0, sizeof(result_blk));
	result_length = 0;
	cout << "S.C" << "\t" << "S.D" << endl;
	int addr = blk1_addr;
	if ((blk = readBlockFromDisk(addr, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
			str[j] = *(blk + 4 + 8 * i + j);
		tmp_C = atoi(str);
		if (tmp_C < 130)
		{
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 + 8 * i + j);
			addr = atoi(str);
		}
		if (tmp_C >= 130)
			break;
	}

	freeBlockInBuffer(blk, &buf);
	if ((blk = readBlockFromDisk(addr, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 4; j++)
			str[j] = *(blk + 8 * i + j);
		tmp_C = atoi(str);
		for (int j = 0; j < 4; j++)
			str[j] = *(blk + 8 * i + j + 4);
		tmp_D = atoi(str);
		if (tmp_C == 130)
		{
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j + 4);
			addr = atoi(str);
			break;
		}
	}
	freeBlockInBuffer(blk, &buf);

	//search
	int flag = 0;
	while (addr != 0)
	{
		if ((blk = readBlockFromDisk(addr, &buf)) == NULL)
		{
			perror("Reading Block Failed!\n");
			return -1;
		}

		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j);
			tmp_C = atoi(str);
			for (int j = 0; j < 4; j++)
				str[j] = *(blk + 8 * i + j + 4);
			tmp_D = atoi(str);
			if (tmp_C == 130)
			{
				flag = 1;
				cout << tmp_C << "\t" << tmp_D << endl;
				sprintf(str, "%d", tmp_C);
				for (int j = 0; j < strlen(str); j++)
					*(result_blk + result_length + j) = str[j];
				sprintf(str, "%d", tmp_D);
				for (int j = 0; j < strlen(str); j++)
					*(result_blk + result_length + j + 4) = str[j];
				result_length += 8;
				if (result_length == 56)
				{
					sprintf(str, "%d", result_addr + 1);
					for (int j = 0; j < strlen(str); j++)
						*(result_blk + 56 + j) = str[j];
					if (writeBlockToDisk(result_blk, result_addr, &buf) != 0)
					{
						perror("Writing Block Failed!\n");
						return -1;
					}
					result_blk = getNewBlockInBuffer(&buf);
					memset(result_blk, 0, sizeof(result_blk));
					result_length = 0;
					result_addr++;
				}
			}
			else if (flag == 1)
				flag = 0;
		}
		for (int j = 0; j < 4; j++)
			str[j] = *(blk + 56 + j);
		addr = atoi(str);
		freeBlockInBuffer(blk, &buf);
		if (flag == 0)
			break;
	}
	sprintf(str, "%d", 0);
	for (int j = 0; j < strlen(str); j++)
		*(result_blk + 56 + j) = str[j];
	if (writeBlockToDisk(result_blk, result_addr, &buf) != 0)
	{
		perror("Writing Block Failed!\n");
		return -1;
	}
	cout << "B+ tree index IO:" << buf.numIO << endl;
	return 0;
}
//sort-merge-join//select S.C, S.D, R.A, R.B from S inner join R on S.C = R.A//result sorted from 500.blk
int sort_merge_join()
{
	int cnt=0;
	Buffer buf;
	unsigned char* blk1, * blk2;
	int R_next = 1000;
	int S_next = 2000;
	int result_blk_addr = 500;
	int tmp_A, tmp_B, tmp_C, tmp_D;
	int tmp_A1, tmp_B1, tmp_C1, tmp_D1;
	char str[5] = { '\0' };
	int r_next_tmp, s_next_tmp;
	unsigned char* tmp_blk;
	unsigned char* result_blk;
	int result_length = 0;

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	result_blk = getNewBlockInBuffer(&buf);
	memset(result_blk, 0, sizeof(result_blk));
	blk1 = readBlockFromDisk(R_next, &buf);
	blk2 = readBlockFromDisk(S_next, &buf);
	int i = 0, j = 0;
	cout << "A" << "\t"<<"B"<<"\t"<<"C"<<"\t"<<"D"<<endl;
	//for each loop ,get the current two elements and compare
	while (R_next != 1016 && S_next != 2032)
	{
		for (int k = 0; k < 4; k++)
			str[k] = *(blk1 + 8 * i + k);
		tmp_A = atoi(str);

		for (int k = 0; k < 4; k++)
			str[k] = *(blk1 + 8 * i + k + 4);
		tmp_B = atoi(str);

		for (int k = 0; k < 4; k++)
			str[k] = *(blk2 + 8 * j + k);
		tmp_C = atoi(str);

		for (int k = 0; k < 4; k++)
			str[k] = *(blk2 + 8 * j + k + 4);
		tmp_D = atoi(str);

		if (tmp_A < tmp_C)
			i++;
		else if (tmp_A > tmp_C)
			j++;

		//if A=C then put current all ABCD into buffer and output 
		else
		{
			sprintf(str, "%d", tmp_A);
			for (int k = 0; k < strlen(str); k++)
				*(result_blk + result_length + k) = str[k];
			sprintf(str, "%d", tmp_B);
			for (int k = 0; k < strlen(str); k++)
				*(result_blk + result_length + k + 4) = str[k];
			sprintf(str, "%d", tmp_C);
			for (int k = 0; k < strlen(str); k++)
				*(result_blk + result_length + k + 8) = str[k];
			sprintf(str, "%d", tmp_D);
			for (int k = 0; k < strlen(str); k++)
				*(result_blk + result_length + k + 12) = str[k];
			cout << tmp_A << "\t" << tmp_B << "\t" << tmp_C << "\t" << tmp_D << endl;
			cnt++;

			//judge wether the buffer is full
			result_length += 16;
			if (result_length == 48)
			{
				sprintf(str, "%d", result_blk_addr + 1);
				for (int k = 0; k < strlen(str); k++)
					*(result_blk + 56 + k) = str[k];
				if (writeBlockToDisk(result_blk, result_blk_addr, &buf) != 0)
				{
					perror("Writing Block Failed!\n");
					return -1;
				}
				result_blk = getNewBlockInBuffer(&buf);
				memset(result_blk, 0, sizeof(result_blk));
				result_length = 0;
				result_blk_addr++;
			}


			/*
			search for C in S where C=A for now
			*/
			int jj = j + 1;
			s_next_tmp = S_next;
			if (jj == 7)
			{
				for (int k = 0; k < 4; k++)
					str[k] = *(blk2 + 56 + k);
				s_next_tmp = atoi(str);
				jj = 0;
			}
			while (s_next_tmp != 2032)
			{
				if ((tmp_blk = readBlockFromDisk(s_next_tmp, &buf)) == NULL)
				{
					perror("Reading Block Failed!\n");
					return -1;
				}
				int flag = 1;
				while (jj < 7)
				{
					for (int k = 0; k < 4; k++)
						str[k] = *(tmp_blk + 8 * jj + k);
					tmp_C1 = atoi(str);
					for (int k = 0; k < 4; k++)
						str[k] = *(tmp_blk + 8 * jj + k + 4);
					tmp_D1 = atoi(str);
					//if find then put all element into buffer
					if (tmp_C1 == tmp_A)
					{
						sprintf(str, "%d", tmp_A);
						for (int k = 0; k < strlen(str); k++)
							*(result_blk + result_length + k) = str[k];
						sprintf(str, "%d", tmp_B);
						for (int k = 0; k < strlen(str); k++)
							*(result_blk + result_length + k + 4) = str[k];
						sprintf(str, "%d", tmp_C1);
						for (int k = 0; k < strlen(str); k++)
							*(result_blk + result_length + k + 8) = str[k];
						sprintf(str, "%d", tmp_D1);
						for (int k = 0; k < strlen(str); k++)
							*(result_blk + result_length + k + 12) = str[k];
						cout << tmp_A << "\t" << tmp_B << "\t" << tmp_C1 << "\t" << tmp_D1 << endl;
						cnt++;

						//judge wether the buffer is full
						result_length += 16;
						if (result_length == 48) {
							sprintf(str, "%d", result_blk_addr + 1);
							for (int k = 0; k < strlen(str); k++)
								*(result_blk + 56 + k) = str[k];
							if (writeBlockToDisk(result_blk, result_blk_addr, &buf) != 0) {
								perror("Writing Block Failed!\n");
								return -1;
							}
							result_blk = getNewBlockInBuffer(&buf);
							memset(result_blk, 0, sizeof(result_blk));
							result_length = 0;
							result_blk_addr++;
						}
					}
					else
						flag = 0;
					jj++;
				}
				for (int k = 0; k < 4; k++) 
					str[k] = *(tmp_blk + 56 + k);
				s_next_tmp = atoi(str);
				jj = 0; 
				freeBlockInBuffer(tmp_blk, &buf);
				if (flag == 0) 
					break;
			}

			/*
			search for A in R where A=A for now
			*/
			int ii = i + 1;
			r_next_tmp = R_next;
			if (ii == 7)
			{
				for (int k = 0; k < 4; k++)
					str[k] = *(blk1 + 56 + k);
				r_next_tmp = atoi(str);
				ii = 0;
			}
			while (r_next_tmp != 1016)
			{
				if ((tmp_blk = readBlockFromDisk(r_next_tmp, &buf)) == NULL)
				{
					perror("Reading Block Failed!\n");
					return -1;
				}
				int flag = 1;
				while (ii < 7)
				{
					for (int k = 0; k < 4; k++)
						str[k] = *(tmp_blk + 8 * ii + k);

					tmp_A1 = atoi(str);
					for (int k = 0; k < 4; k++) {
						str[k] = *(tmp_blk + 8 * ii + k + 4);
					}
					tmp_B1 = atoi(str);
					if (tmp_A1 == tmp_C)
					{
						sprintf(str, "%d", tmp_A1);
						for (int k = 0; k < strlen(str); k++)
							*(result_blk + result_length + k) = str[k];
						sprintf(str, "%d", tmp_B1);
						for (int k = 0; k < strlen(str); k++)
							*(result_blk + result_length + k + 4) = str[k];
						sprintf(str, "%d", tmp_C);
						for (int k = 0; k < strlen(str); k++)
							*(result_blk + result_length + k + 8) = str[k];
						sprintf(str, "%d", tmp_D);
						for (int k = 0; k < strlen(str); k++)
							*(result_blk + result_length + k + 12) = str[k];
						cout << tmp_A1 << "\t" << tmp_B1 << "\t" << tmp_C << "\t" << tmp_D << endl;
						cnt++;
						result_length += 16;
						if (result_length == 48)
						{
							sprintf(str, "%d", result_blk_addr + 1);
							for (int k = 0; k < strlen(str); k++)
								*(result_blk + 56 + k) = str[k];
							if (writeBlockToDisk(result_blk, result_blk_addr, &buf) != 0)
							{
								perror("Writing Block Failed!\n");
								return -1;
							}
							result_blk = getNewBlockInBuffer(&buf);
							memset(result_blk, 0, 64);
							result_length = 0;
							result_blk_addr++;
						}
					}
					else
						flag = 0;
					ii++;
				}
				for (int k = 0; k < 4; k++)
					str[k] = *(tmp_blk + 56 + k);
				r_next_tmp = atoi(str);
				ii = 0;
				freeBlockInBuffer(tmp_blk, &buf);
				if (flag == 0)
					break;
			}
			i++;
			j++;
		}
		if (i == 7)
		{
			for (int k = 0; k < 4; k++)
				str[k] = *(blk1 + 56 + k);
			R_next = atoi(str);
			i = 0;
			freeBlockInBuffer(blk1, &buf);
			if (R_next != 1016)
			{
				if ((blk1 = readBlockFromDisk(R_next, &buf)) == NULL)
				{
					perror("Reading Block Failed!\n");
					return -1;
				}
			}
		}

		if (j == 7)
		{
			for (int k = 0; k < 4; k++)
				str[k] = *(blk2 + 56 + k);
			S_next = atoi(str);
			j = 0;
			freeBlockInBuffer(blk2, &buf);
			if (S_next != 2032)
			{
				if ((blk2 = readBlockFromDisk(S_next, &buf)) == NULL)
				{
					perror("Reading Block Failed!\n");
					return -1;
				}
			}
		}

		if (result_length == 56)
		{
			if (R_next == 1016 && S_next == 2032)
				sprintf(str, "%d", 0);
			else
				sprintf(str, "%d", result_blk_addr + 1);
			for (int k = 0; k < strlen(str); k++)
				*(result_blk + 56 + k) = str[k];
			if (writeBlockToDisk(result_blk, result_blk_addr, &buf) != 0)
			{
				perror("Writing Block Failed!\n");
				return -1;
			}
			result_blk = getNewBlockInBuffer(&buf);
			memset(result_blk, 0, sizeof(result_blk));
			result_length = 0;
			result_blk_addr++;
		}
	}
	sprintf(str, "%d", 0);
	for (int k = 0; k < strlen(str); k++)
		*(result_blk + 56 + k) = str[k];
	if (writeBlockToDisk(result_blk, result_blk_addr, &buf) != 0)
	{
		perror("Writing Block Failed!\n");
		return -1;
	}
	cout << "IO:" << buf.numIO << endl;
	cout << "cnt:" << cnt << endl;
	return 0;
}
//RUS
int R_union_S()
{
	Buffer buf;
	unsigned char* blk;
	char str[5] = { '\0' };
	int blk1_addr = 1000;
	int blk2_addr = 2000;
	int tmp_AC1, tmp_BD1, tmp_AC2, tmp_BD2;
	int result_addr = 5000;
	int result_length = 0;
	unsigned char* blk1, * blk2;
	int cnt = 0;
	int i = 0, j = 0;
	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	blk = getNewBlockInBuffer(&buf);
	memset(blk, 0, sizeof(blk));
	blk1 = readBlockFromDisk(blk1_addr, &buf);
	blk2 = readBlockFromDisk(blk2_addr, &buf);
	cout << "RS.AC" << "\t" << "RS.BD" << endl;
	while (blk1_addr != 1016 || blk2_addr != 2032)
	{
		for (int k = 0; k < 4; k++)
			str[k] = *(blk1 + 8 * i + k);
		tmp_AC1 = atoi(str);
		for (int k = 0; k < 4; k++)
			str[k] = *(blk1 + 8 * i + k + 4);
		tmp_BD1 = atoi(str);


		for (int k = 0; k < 4; k++)
			str[k] = *(blk2 + 8 * j + k);
		tmp_AC2 = atoi(str);
		for (int k = 0; k < 4; k++)
			str[k] = *(blk2 + 8 * j + k + 4);
		tmp_BD2 = atoi(str);

		if (blk1_addr == 1016)
		{
			j++;
			sprintf(str, "%d", tmp_AC2);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k) = str[k];
			sprintf(str, "%d", tmp_BD2);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k + 4) = str[k];
			result_length += 8;
			cout << tmp_AC2 << "\t" << tmp_BD2 << endl;
			cnt++;
		}
		else if (blk2_addr ==2032)
		{
			i++;
			sprintf(str, "%d", tmp_AC1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k) = str[k];
			sprintf(str, "%d", tmp_BD1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + result_length + k + 4) = str[k];
			result_length += 8;
			cout << tmp_AC1 << "\t" << tmp_BD1 << endl;
			cnt++;
		}
		else
		{
			if (tmp_AC1 <= tmp_AC2)
			{
				i++;
				sprintf(str, "%d", tmp_AC1);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k) = str[k];
				sprintf(str, "%d", tmp_BD1);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k + 4) = str[k];
				result_length += 8;
				cout << tmp_AC1 << "\t" << tmp_BD1 << endl;
				cnt++;
			}
			else
			{
				j++;
				sprintf(str, "%d", tmp_AC2);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k) = str[k];
				sprintf(str, "%d", tmp_BD2);
				for (int k = 0; k < strlen(str); k++)
					*(blk + result_length + k + 4) = str[k];
				result_length += 8;
				cout << tmp_AC2 << "\t" << tmp_BD2 << endl;
				cnt++;
			}
		}

		if (i == 7)
		{
			for (int k = 0; k < 4; k++)
				str[k] = *(blk1 + 56 + k);
			blk1_addr = atoi(str);
			i = 0;
			freeBlockInBuffer(blk1, &buf);
			if (blk1_addr != 1016)
			{
				if ((blk1 = readBlockFromDisk(blk1_addr, &buf)) == NULL)
				{
					perror("Reading Block Failed\n");
					return -1;
				}
			}
		}
		if (j == 7)
		{
			for (int k = 0; k < 4; k++)
				str[k] = *(blk2 + 56 + k);
			blk2_addr = atoi(str);
			j = 0;
			freeBlockInBuffer(blk2, &buf);
			if (blk2_addr != 2032)
			{
				if ((blk2 = readBlockFromDisk(blk2_addr, &buf)) == NULL)
				{
					perror("Reading Block Failed\n");
					return -1;
				}
			}
		}
		if (result_length == 56)
		{
			if (blk1_addr == 0 && blk2_addr == 0)
				sprintf(str, "%d", 0);
			else
				sprintf(str, "%d", result_addr + 1);
			for (int k = 0; k < strlen(str); k++)
				*(blk + 56 + k) = str[k];

			if (writeBlockToDisk(blk, result_addr, &buf) != 0)
			{
				perror("Writing Block Failed!\n");
				return -1;
			}
			blk = getNewBlockInBuffer(&buf);
			memset(blk, 0, sizeof(blk));
			result_length = 0;
			result_addr++;
		}
	}
	cout << "cnt:" << cnt << endl;
	return 0;
}
