#include <mpi.h>
#include <math.h>
#include <stdio.h>

double f(const double a)
{
	return 4.0 / (1.0 + a * a);
}
int main(int argc, char* argv[])
{
	int done = 0;
	int n = 0;
	int my_id = 0;
	int num_proc = 0;
	const double pi_25dt = 3.141592653589793238462643;
	double my_pi = NAN;
	double pi = NAN;
	double start_time = 0.0;
	int  name_len = 0;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	MPI_Get_processor_name(processor_name, &name_len);
	while (!done)
	{
		if (my_id == 0)
		{
			fprintf(stdout, "Enter the number of intervals: (0 quits) "); fflush(stdout);
			if (scanf_s("%d", &n) != 1)
			{
				fprintf(stdout, "No number entered; quitting\n");  n = 0;
			}
			start_time = MPI_Wtime();
		}
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (n == 0)  done = 1;
		else {
			const double h = 1.0 / (double)n;
			double sum = 0.0;
			for (int i = my_id + 1; i <= n; i += num_proc)
			{
				const double x = h * ((double)i - 0.5);
				sum += f(x);
			}
			my_pi = h * sum;
			MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

			if (my_id == 0) {
				printf("pi is approximately %.16f, Error is %.16f\n",
					pi, fabs(pi - pi_25dt));
				const double end_time = MPI_Wtime();
				printf("wall clock time = %f\n", end_time - start_time);
				fflush(stdout);
			}
		}
	}
	MPI_Finalize();
	return 0;
}