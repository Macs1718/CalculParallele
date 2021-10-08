#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int nargs, char* argv[])
{
    char filename[256];
    int rank, nbp;
    MPI_Init(&nargs, &argv);
    MPI_Comm world;
    MPI_Comm_dup(MPI_COMM_WORLD, &world);

    MPI_Comm_rank(world, &rank);
    MPI_Comm_size(world, &nbp);

    printf("%d : Hello world\n", rank);
    fflush(stdout);

    sprintf(filename, "output%03d.txt", rank);
    FILE* fich = fopen(filename, "w");
    fprintf(fich, "Hello world from %d/%d\n", rank, nbp);

    if (rank == 0)
    {
        fprintf(fich, "Je suis le root\n");
    }

    if (rank == 0)
    {
        int val = (rank+1)*2+1;
        MPI_Send(&val, 1, MPI_INTEGER, 1, 101, world);
    }
    else if (rank == 1)
    {
        MPI_Status status;
        int val, err;

        err = MPI_Recv(&val,1,MPI_INTEGER, 0, 101, world, &status);
        if (err != MPI_SUCCESS)
        {
            char* errormsg;
            int lenmsg;
            fprintf(fich, "Erreur du message provenant de %d  avec le n° %d :", status.MPI_SOURCE, status.MPI_TAG);
            MPI_Error_string(status.MPI_ERROR, NULL, &lenmsg);
            errormsg = malloc(lenmsg);
            MPI_Error_string(status.MPI_ERROR, errormsg, &lenmsg);
            fprintf(fich, "%s\n", errormsg);
            free(errormsg);
        }
        fprintf(fich, "valeur recue : %d\n", val);
    }

    if (rank > 0)
    {
        MPI_Status status;
        int val;
        MPI_Recv(&val, 1, MPI_INTEGER, rank-1, 101, world, &status);
        printf("%d : val vaut %d\n", rank, val);
        val += 1;
        MPI_Send(&val, 1, MPI_INTEGER, (rank+1)%nbp, 101, world);
        fflush(stdout);
    }
    else
    {
        MPI_Status status;
        int val = 1;
        MPI_Send(&val, 1, MPI_INTEGER, 1, 101, world);
        MPI_Recv(&val, 1, MPI_INTEGER, nbp-1, 101, world, &status);
        printf("%d : val vaut %d\n", rank, val);
        fprintf(fich, "val = %d\n", val);
        fflush(stdout);
    }

    fclose(fich);
    MPI_Finalize();
    return EXIT_SUCCESS;
}