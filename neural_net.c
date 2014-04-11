#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct Neuron {
	struct Dendrite* outgoing_list[10];
	int layer;
	double output;
	double error;
} Neuron ;

typedef struct Dendrite {
	double weight;
	struct Neuron* target;
} Dendrite ;

typedef struct Layer {
	struct Neuron* neuron_list[10];
} Layer ;

double sigmoid(double x) {
	return x / (1 + abs(x));
}

double d_sigmoid(double x) {
   double s = sigmoid(x);
   return s * (1 - s);
}


void fire_neuron(Neuron* neuron) {
	for(int i=0;i<10;i++) {
		Dendrite* d = neuron->outgoing_list[i];
		if(d) {
			Neuron* n = d->target;
			double weight = d->weight;
			n->output += (neuron->output * weight);
		}
	}
}

void calculate_weight(Neuron* neuron) {
	double post_synaptic_error = 0;
	for(int i=0;i<10;i++) {
		Dendrite* d = neuron->outgoing_list[i];
		if(d) {
			Neuron* n = d->target;
			post_synaptic_error += (n->error * d->weight);
		}
	}

	neuron->error = post_synaptic_error;

	for(int i=0;i<10;i++) {
		Dendrite* d = neuron->outgoing_list[i];
		if(d) {
			d->weight = (d->weight + (d->target->error * d_sigmoid(d->target->error) * neuron->output));
			printf("Weight: %f\n", d->weight);
		}
	}
}

void fire_layer(Layer* layer) {
	for(int i=0;i<10;i++) {
		Neuron* n = layer->neuron_list[i];
		if(n) {
			fire_neuron(n);
		}
	}
}

void back_propgate_layer(Layer* layer) {
	for(int i=0;i<10;i++) {
		Neuron* n = layer->neuron_list[i];
		if(n) {
			calculate_weight(n);
		}
	}
}


int main(void) {
	srand(time(NULL));

	Neuron n4 = {{0}, 3};
	Neuron n3 = {{&(Dendrite){(double)rand()/rand(), &n4}}, 2};
	Neuron n2 = {{&(Dendrite){(double)rand()/rand(), &n4}}, 1};
	Neuron n1 = {{&(Dendrite){(double)rand()/rand(), &n3},&(Dendrite){(double)rand()/rand(), &n2}}, 0};


	Layer in = { {&n1} };
	Layer hidden = { {&n2,&n3} };
	Layer out = { {&n4} };

	float expected_output = 0.5;

	for(int i = 0;i<10;i++) {
		n1.output = 0.5;

		fire_layer(&in);
		fire_layer(&hidden);
		fire_layer(&out);

		n4.error = n4.output - expected_output;
		back_propgate_layer(&hidden);
		back_propgate_layer(&in);

		n1.output = 0;
		n2.output = 0;
		n3.output = 0;
		n4.output = 0;

		printf("\nError: %f\n", n4.output - expected_output);
	}

}
