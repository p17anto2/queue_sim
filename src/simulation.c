/* 
 * Author: Antonakakis Apostolos
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX  10e5
#define STEP 1000
#define K    6
#define CUTOFF 400
#define PLOT 1

/* Contains all simulation variables */
typedef struct
{
    double arr_rate;
    double a_dep_rate;
    double b_dep_rate;
    double total_dep_rate;
} Sim_opts;

/* Given the arrival rate, the departure rates and a pointer to a plotting function,
 * perform the simulation */
int simulation(double arr_rate, double a_dep_rate, double b_dep_rate, int (*plot_function)(Sim_opts,int**));
/* Perform a simulation step */
int* simulation_step(Sim_opts opts, int *state_arrivals, bool is_last);
/* Plot functions for writing plot points to a file */
int plot_average(Sim_opts opts, int **state_arrivals);
int plot_variable_arr(Sim_opts opts, int **state_arrivals);
int plot_variable_dep(Sim_opts opts, int **state_arrivals);
/* Free allocated memory for state_arrivals */
void free_resources(int **state_arrivals);

int main()
{
    double i, j;
    FILE *gplot;

    /* Initialise random seed */
    srand(time(0));

    /* Simulation 1: Figure out unstable period. The switch is for sanity checks */
    switch(simulation(4.0, 3.0, 5.0, &plot_average))
    {
        case 0:
            break;
        case 1:
            printf("Sim 1: Memory allocation error\n");
            return 1;
        case 2:
            printf("Sim 1: General simulation error\n");
            return 1;
        case 3:
            printf("Sim 1: Arrival rate is greater than total departure rate\n");
            return 1;
        case 4:
            printf("Sim 1: File IO error\n");
            return 1;
        default:
            printf("Sim 1: Unknown return code\n");
            return 1;
    }
    
    /* Simulation 2: Variable arrival rate, constant departure rates */
    for (i = 1; i < 13; i++)
    {
        switch(simulation(i, 5.0, 8.0, &plot_variable_arr))
        {
            case 0:
                break;
            case 1:
                printf("Sim 2-%1.f: Memory allocation error\n", i);
                return 1;
            case 2:
                printf("Sim 2-%1.f: General simulation error\n", i);
                return 1;
            case 3:
                printf("Sim 2-%1.f: Arrival rate is greater than total departure rate\n", i);
                return 1;
            case 4:
                printf("Sim 2-%1.f: File IO error\n",i);
                return 1;
            default:
                printf("Sim 2-%1.f: Unknown return code\n", i);
                return 1;   
        }
    }

    /* Simulation 3: Variable departure rates, constant arrival rate */
    for (i = 3.0; i <= 5.0; i++)
    {
        for (j = 6.0; j <= 12.0; j+=3)
        {
            switch (simulation(8.0, i, j, &plot_variable_dep))
            {
                case 0:
                    break;
                case 1:
                    printf("Sim 3-%1.f/%1.f: Memory allocation error\n", i, j);
                    return 1;
                case 2:
                    printf("Sim 2-%1.f/%1.f: General simulation error\n", i, j);
                    return 1;
                case 3:
                    printf("Sim 2-%1.f/%1.f: Arrival rate is greater than total departure rate\n", i, j);
                    return 1;
                case 4:
                    printf("Sim 2-%1.f/%1.f: File IO error\n", i, j);
                    return 1;
                default:
                    printf("Sim 2-%1.f/%1.f: Unknown return code\n", i, j);
                    return 1;   
            }
        }
    }

    if (PLOT)
    {
        /* Plot and save to png with gnuplot */
        gplot = popen("gnuplot", "w");
        if (gplot == NULL) {printf("Can't open gnuplot\n"); return 1;}

        /* Plot simulation 1 */
        fprintf(gplot, "set title 'Averages (λ = 4, μ_a = 3, μ_b = 5) '\n");
        fprintf(gplot, "set xlabel 'Steps'\n");
        fprintf(gplot, "set ylabel 'Average State Probabilty'\n");
        fprintf(gplot, "set datafile commentschars '#'\n");
        fprintf(gplot, "set style data lp\n");
        fprintf(gplot, "set term png\n");
        fprintf(gplot, "set output 'averages.png'\n");
        fprintf(gplot, "plot 'averages.pdata' notitle\n");
        fprintf(gplot, "clear\n");

        /* Plot Simulation 2 */
        fprintf(gplot, "set title 'Variable Arrival Rate (μ_a = 5, μ_b = 8)'\n");
        fprintf(gplot, "set xlabel 'λ'\n");
        fprintf(gplot, "set ylabel ''\n");
        fprintf(gplot, "set datafile commentschars '#'\n");
        fprintf(gplot, "set style data lp\n");
        fprintf(gplot, "set term png\n");
        fprintf(gplot, "set output 'var_arr.png'\n");
        fprintf(gplot, "plot 'var_arr.pdata' using 1:2 t 'Average State',");
        fprintf(gplot, "'var_arr.pdata' using 1:3 t 'Throughput'\n");
        fprintf(gplot, "clear\n");

        /* Plot Simulation 3 - Average State */
        fprintf(gplot, "set title 'Variable Departure Rates (λ = 8)'\n");
        fprintf(gplot, "set xlabel 'μ_a'\n");
        fprintf(gplot, "set ylabel 'μ_b'\n");
        fprintf(gplot, "set zlabel ''\n");
        fprintf(gplot, "set datafile commentschars '#'\n");
        fprintf(gplot, "set style data lp\n");
        fprintf(gplot, "set term png\n");
        fprintf(gplot, "set output 'var_dep_average.png'\n");
        fprintf(gplot, "splot 'var_dep.pdata' using 1:2:3 t 'Average State'\n");
        fprintf(gplot, "clear\n");

        /* Plot Simulation 3 - Throughput */
        fprintf(gplot, "set title 'Variable Departure Rates (λ = 8)'\n");
        fprintf(gplot, "set xlabel 'μ_a'\n");
        fprintf(gplot, "set ylabel 'μ_b'\n");
        fprintf(gplot, "set zlabel ''\n");
        fprintf(gplot, "set datafile commentschars '#'\n");
        fprintf(gplot, "set style data lp\n");
        fprintf(gplot, "set term png\n");
        fprintf(gplot, "set output 'var_dep_throughput.png'\n");
        fprintf(gplot, "splot 'var_dep.pdata' using 1:2:4 t 'Throughput'\n");
        fprintf(gplot, "clear\n");

        pclose(gplot);
        gplot = NULL;
    }
}

int simulation(double arr_rate, double a_dep_rate, double b_dep_rate, int (*plot_function)(Sim_opts,int**))
{
    int i, j;

    /* Initialise simulation options */
    Sim_opts opts;
    opts.arr_rate = arr_rate;
    opts.a_dep_rate = a_dep_rate;
    opts.b_dep_rate = b_dep_rate;
    opts.total_dep_rate = a_dep_rate + b_dep_rate;

    /* If the total departure rate is less than the arrival rate, ρ > 1 */
    if(opts.arr_rate > opts.total_dep_rate) return 3;
    
    /* state_arrivals is an array of pointers (for each step), each pointing
     * to an array of 11 pointers for each state with the amount of times
     * each state was visited */
    int **state_arrivals = malloc((MAX/STEP) * sizeof *state_arrivals);
    if (state_arrivals == NULL) return 1;

    /* Safe memory allocation of the arrays. If an error occurs during
     * allocation, the program frees the allocated memory up to that 
     * point */
    for (i = 0; i < MAX/STEP; i++)
    {
        state_arrivals[i] = malloc(11 * sizeof *state_arrivals[i]);
        if (state_arrivals[i] == NULL)
        {
            for (j = i-1; j>= 0; j--)
            {
                free(state_arrivals[j]);
                state_arrivals[j] = NULL;
            }

            free(state_arrivals);
            state_arrivals = NULL;
            return 1;
        }
    }

    /* If the next step will be the last */
    bool is_last = false;

    for (i = 0; i < MAX/STEP; i++)
    {
        if(i+1 >= MAX/STEP) is_last = true;
        /* Perform a simulation step, and write the results to the array */
        state_arrivals[i] = simulation_step(opts, state_arrivals[i], is_last);
        if (state_arrivals[i] == NULL){ free_resources(state_arrivals); return 2;} 
    }

    int plot_ret = (*plot_function)(opts, state_arrivals);
    if(plot_ret) {free_resources(state_arrivals); return plot_ret;}

    free_resources(state_arrivals);
    return 0;
}

int* simulation_step(Sim_opts opts, int *state_arrivals, bool is_last)
{
    static int cur_state = 0;
    int i, count = 0;
    double threshold;

    /* Initialise all arrays to 0 */
    for (i = 0; i < 11; i++){state_arrivals[i] = 0;}

    while (count < STEP)
    {
        /* Check if the second server should wake up and set
         * the threshold accordingly*/
        if (cur_state < K)
        {
            // B is asleep
            threshold = opts.arr_rate / (opts.arr_rate + opts.a_dep_rate);
        }
        else
        {
            // B is awake
            threshold = opts.arr_rate / (opts.arr_rate + opts.total_dep_rate);
        }

        /* If the current state is 0, only arrivals are allowed */
        if (cur_state == 0)
        {
            // Arrival
            state_arrivals[cur_state]++;
            count++;
            cur_state++;
        }
        else
        {
            double random_number = (double)rand()/(double)RAND_MAX;
            if (random_number < threshold && cur_state <= 10)
            {
                // Arrival
                state_arrivals[cur_state]++;
                count++;
                cur_state++;
            }
            else
            {
                // Departure
                count++;
                cur_state--;
            }
        }
    }

    /* Because cur_state is static, we have to check
     * if this is the last step so that it won't interfere
     * with following simulations */
    if(is_last == true) cur_state = 0; 

    return(state_arrivals);
}

int plot_average(Sim_opts opts, int **state_arrivals)
{
    int i, j, total_arrivals = 0;

    /* Array for the total arrivals of each state */
    int *total_state_arrivals = malloc(11 * sizeof *total_state_arrivals);
    if (total_state_arrivals == NULL) return 1;
    for (i = 0; i < 11; i++){total_state_arrivals[i] = 0;}

    /* Array for the averages of each step */
    double *averages = malloc((MAX/STEP) * sizeof *averages);
    if (averages == NULL){free(total_state_arrivals); return 1;}

    /* The file to write the data to */
    FILE *f = fopen("averages.pdata", "w");
    if (f == NULL) {free(averages); free(total_state_arrivals); return 4;}

    for (i = 0; i < MAX/STEP; i++)
    {
        averages[i] = 0;
        for (j = 0; j < 11; j++)
        {
            total_state_arrivals[j] += state_arrivals[i][j];
            total_arrivals += state_arrivals[i][j];
        }
        
        // Calculate Probabilities
        for (j = 0; j < 11; j++)
        {
            double state_probability = (double)total_state_arrivals[j] / (double)total_arrivals;
            averages[i] += (double)j * state_probability;
        }
    }

    /* Write to file */
    for (int i = 0; i < MAX/STEP; i++)
    {
        fprintf(f, "%d\t %f\n", i, averages[i]);
    }

    fclose(f);
    f = NULL;

    free(averages);
    averages = NULL;

    free(total_state_arrivals);
    total_state_arrivals = NULL;

    return 0;
}

int plot_variable_arr(Sim_opts opts, int **state_arrivals)
{
    int i, j, total_arrivals = 0;
    double average, last_state_average;
    double average_total = 0.0, last_state_total = 0.0, count = 0.0;

    /* Array for the total arrivals of each state */
    int *total_state_arrivals = malloc(11 * sizeof *total_state_arrivals);
    if (total_state_arrivals == NULL) return 1;
    for (i = 0; i < 11; i++){total_state_arrivals[i] = 0;}

    /* Array for the averages of each step */
    double *averages = malloc((MAX/STEP) * sizeof *averages);
    if (averages == NULL){free(total_state_arrivals); return 1;}

    double *last_state_probabilities = malloc(((MAX/STEP) - CUTOFF) * sizeof *last_state_probabilities);
    if (last_state_probabilities == NULL) {free(total_state_arrivals); free(averages); return 1;}

    /* The file to write the data to */
    FILE *f = fopen("var_arr.pdata", "a");
    if (f == NULL) {free(last_state_probabilities); free(averages); free(total_state_arrivals); return 4;}

    for (i = 0; i < MAX/STEP; i++)
    {
        averages[i] = 0;
        for (j = 0; j < 11; j++)
        {
            total_state_arrivals[j] += state_arrivals[i][j];
            total_arrivals += state_arrivals[i][j];
        }
        
        // Calculate Probabilities
        for (j = 0; j < 11; j++)
        {
            double state_probability = (double)total_state_arrivals[j] / (double)total_arrivals;
            averages[i] += (double)j * state_probability;

            /* Keep the probability of the blocking state */
            if(i >= CUTOFF && j == 10)
            {
                last_state_probabilities[i - CUTOFF] = state_probability;
            }
        }
    }

    /* Calculate the average of all the step averages and the blocking state probabilties
     * after the cutoff point */
    for (i = CUTOFF; i < MAX/STEP; i++)
    {
        average_total += averages[i];
        last_state_total += last_state_probabilities[i - CUTOFF];
        count++;
    }
    average = average_total / count;
    last_state_average = last_state_total / count;

    fprintf(f, "%f\t %f\t %f\n", opts.arr_rate, average, opts.arr_rate * (1 - last_state_average));

    fclose(f);
    f = NULL;

    free(last_state_probabilities);
    last_state_probabilities = NULL;

    free(averages);
    averages = NULL;

    free(total_state_arrivals);
    total_state_arrivals = NULL;

    return 0;
}

int plot_variable_dep(Sim_opts opts, int **state_arrivals)
{
    int i, j, total_arrivals = 0;
    double average, last_state_average;
    double average_total = 0.0, last_state_total = 0.0, count = 0.0;

    /* Array for the total arrivals of each state */
    int *total_state_arrivals = malloc(11 * sizeof *total_state_arrivals);
    if (total_state_arrivals == NULL) return 1;
    for (i = 0; i < 11; i++){total_state_arrivals[i] = 0;}

    /* Array for the averages of each step */
    double *averages = malloc((MAX/STEP) * sizeof *averages);
    if (averages == NULL){free(total_state_arrivals); return 1;}

    double *last_state_probabilities = malloc(((MAX/STEP) - CUTOFF) * sizeof *last_state_probabilities);
    if (last_state_probabilities == NULL) {free(total_state_arrivals); free(averages); return 1;}

    /* The file to write the data to */
    FILE *f = fopen("var_dep.pdata", "a");
    if (f == NULL) {free(last_state_probabilities); free(averages); free(total_state_arrivals); return 4;}

    for (i = 0; i < MAX/STEP; i++)
    {
        averages[i] = 0;
        for (j = 0; j < 11; j++)
        {
            total_state_arrivals[j] += state_arrivals[i][j];
            total_arrivals += state_arrivals[i][j];
        }
        
        // Calculate Probabilities
        for (j = 0; j < 11; j++)
        {
            double state_probability = (double)total_state_arrivals[j] / (double)total_arrivals;
            averages[i] += (double)j * state_probability;

            /* Keep the probability of the blocking state */
            if(i >= CUTOFF && j == 10)
            {
                last_state_probabilities[i - CUTOFF] = state_probability;
            }
        }
    }

    /* Calculate the average of all the step averages and the blocking state probabilties
     * after the cutoff point */
    for (i = CUTOFF; i < MAX/STEP; i++)
    {
        average_total += averages[i];
        last_state_total += last_state_probabilities[i - CUTOFF];
        count++;
    }
    average = average_total / count;
    last_state_average = last_state_total / count;


    /* Write points to file */
    fprintf(f, "%f\t %f\t %f\t %f\n", opts.a_dep_rate, opts.b_dep_rate, average,
            opts.arr_rate * (1 - last_state_average));

    fclose(f);
    f = NULL;

    free(last_state_probabilities);
    last_state_probabilities = NULL;

    free(averages);
    averages = NULL;

    free(total_state_arrivals);
    total_state_arrivals = NULL;

    return 0;

}

void free_resources(int **state_arrivals)
{
    for (int i = 0; i < MAX/STEP; i++)
    {
        free(state_arrivals[i]);
        state_arrivals[i] = NULL;
    }
    free(state_arrivals);
    state_arrivals = NULL;
}
