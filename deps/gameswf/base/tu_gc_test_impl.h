// tu_gc_test_impl.h  -- Thatcher Ulrich <http://tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some inline implementation of garbage collector test functions.
// This is designed to be a nested include, parameterized by the macro
// GC_COLLECTOR.

DECLARE_GC_TYPES(GC_COLLECTOR);

struct cons : public gc_object {
public:
	cons() : value(0) {
	}
	cons(int val, cons* cdr_in) : value(val), cdr(cdr_in) {
	}
	cons(cons* car_in, cons* cdr_in) : value(0), car(car_in), cdr(cdr_in) {
	}

	int value;
	gc_ptr<cons> car;
	gc_ptr<cons> cdr;
};

gc_ptr<cons> build_list_of_numbers(int low, int high) {
	if (low <= high) {
		return new cons(low, build_list_of_numbers(low + 1, high).get());
	} else {
		return NULL;
	}
}

// Classic cycle; ref-counting would not be able to collect this.
gc_ptr<cons> build_cycle(int size) {
	gc_ptr<cons> head = new cons(-1, NULL);
	gc_ptr<cons> tail = head;
	for (int i = 0; i < size - 1; i++) {
		head = new cons(i, head.get());
	}
	tail->cdr = head;  // close the loop.
	return head;
}

void dump_sub(const gc_ptr<cons>& node, std::set<cons*>* visited) {
	if (node == NULL) {
		printf("nil");
		return;
	}

	if (visited->find(node.get()) != visited->end()) {
		// We've been here before; don't loop infinitely.
		printf("cycle");
		return;
	}
	visited->insert(node.get());
	
	if (node->car == NULL) {
		printf("%d", node->value);
	} else {
		printf("( ");
		dump_sub(node->car, visited);
		printf(" )");
	}
	if (node->cdr == NULL) {
		// nada
	} else {
		printf(" . (");
		dump_sub(node->cdr, visited);
		printf(")");
	}
}

void dump(const gc_ptr<cons>& node) {
	std::set<cons*> visited;
	dump_sub(node, &visited);
	printf("\n");
}

void test_basic_stuff() {
	gc_ptr<cons> root1 = build_list_of_numbers(1, 10);
	gc_ptr<cons> root2 = build_cycle(20);

	dump(root1);
	dump(root2);

	gc_collector::stats s;
	gc_collector::get_stats(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);

 	printf("collecting...\n");
	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);

	dump(root1);
	dump(root2);

	root1 = NULL;

 	printf("collecting...\n");
	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);

	dump(root1);
	dump(root2);

	root2 = NULL;
	
	printf("collecting...\n");
	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);
}

// test multiple inheritance (objects & mix-ins virtually inherit from gc_object)

struct actor : virtual public gc_object {
	gc_ptr<actor> m_parent;
};

// a "mix-in"
struct space_physics : virtual public actor {
};

struct vehicle : virtual public actor {
};

// another "mix-in"
struct steerable : virtual public vehicle {
};

struct spaceship : virtual public space_physics, virtual public steerable {
};

struct tractor_beam : virtual public actor {
	gc_ptr<vehicle> m_target;
};

void test_multiple_inheritance() {
	printf("\nmultiple inheritance\n");

	gc_collector::stats s;
	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);

	gc_ptr<actor> ship = new spaceship;
	gc_ptr<tractor_beam> beam = new tractor_beam;
	beam->m_target = dynamic_cast<vehicle*>(ship.get());

	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);

	ship = NULL;

	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);

	beam = NULL;

	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);
}

// test gc_container

struct bag : public gc_object {
	GC_CONTAINER(std::vector, bag) m_ptrs;
	// or, without the helper macro:
	//   gc_container<std::vector<contained_gc_ptr<bag> > > m_ptrs;
};

void test_gc_container() {
	printf("\ncontainer\n");
	
	gc_ptr<bag> b = new bag;
	b->m_ptrs.push_back(new bag);
	b->m_ptrs.push_back(new bag);
	b->m_ptrs.push_back(new bag);
	b->m_ptrs[1]->m_ptrs.push_back(new bag);
	b->m_ptrs[1]->m_ptrs[0]->m_ptrs.push_back(b.get());

	gc_collector::stats s;
	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);

	// mark-sweep should correctly collect the group of objects here.
	// ref-counted should leak, because there is a cycle.
	b = NULL;

	gc_collector::collect_garbage(&s);
	printf("%d %d %d %d\n", s.live_heap_bytes, s.garbage_bytes, s.root_pointers, s.live_pointers);
}

void run_tests() {
	test_basic_stuff();
	test_multiple_inheritance();
	test_gc_container();
}

