const int chunk_size = 300;
class world 
{
private:
	
public:
	
	int **world_proh, **world_texture, seed, chunk[3][3][chunk_size][chunk_size];

	void init(int x,int y, int sd) {
		int i;
		seed = sd;
		world_proh = new int* [y];
		world_texture = new int* [y];
		for (i = 0; i < y; i++)
		{
			world_proh[i] = new int[x];
			world_texture[i] = new int[x];
		}
	}
	void create_chunk(int x, int y)
	{

	}
};