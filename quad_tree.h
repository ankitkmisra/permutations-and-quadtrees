#include <iostream>

class quad_tree
{
	struct quad_node
	{
		int val, xmin, xmax, ymin, ymax;
		quad_node *subnode[4];
	};

	quad_node *root;
	int n;

	void killChildren(quad_node *node)
	{
		if (node->val >= 0)
			return;
		for (int i = 0; i < 4; i++) {
			killChildren(node->subnode[i]);
			delete node->subnode[i];
		}
	}

	void copyQuad(quad_node *n2, quad_node *node)
	{
		n2->val = node->val;
		n2->xmin = node->xmin;
		n2->xmax = node->xmax;
		n2->ymin = node->ymin;
		n2->ymax = node->ymax;
		if (node->val == -1)
			for (int i = 0; i < 4; i++) {
				n2->subnode[i] = new quad_node();
				copyQuad(n2->subnode[i], node->subnode[i]);
			}
	}

	void setVal(quad_node *node, int x1, int y1, int x2, int y2, int b)
	{
		if (node->xmin >= x1 && node->xmax <= x2 && node->ymin >= y1 && node->ymax <= y2) {
			if (node->val == -1)
				killChildren(node);
			node->val = b;
		}
		else {
			if (node->val != -1) {
				for (int i = 0; i < 4; i++) {
					node->subnode[i] = new quad_node();
					node->subnode[i]->val = node->val;
					if (i <= 1) {
						node->subnode[i]->xmin = node->xmin;
						node->subnode[i]->xmax = (node->xmax + node->xmin)/2;
					}
					else {
						node->subnode[i]->xmin = (node->xmax + node->xmin)/2 + 1;
						node->subnode[i]->xmax = node->xmax;
					}
					if (i == 0 || i == 3) {
						node->subnode[i]->ymin = node->ymin;
						node->subnode[i]->ymax = (node->ymax + node->ymin)/2;
					}
					else {
						node->subnode[i]->ymin = (node->ymax + node->ymin)/2 + 1;
						node->subnode[i]->ymax = node->ymax;
					}
				}
				node->val = -1;
			}
			for (int i = 0; i < 4; i++) {
				if (node->subnode[i]->xmax >= x1 && node->subnode[i]->xmin <= x2 && node->subnode[i]->ymin <= y2 && node->subnode[i]->ymax >= y1)
					setVal(node->subnode[i], x1, y1, x2, y2, b);
			}
			int tmpval = node->subnode[0]->val;
			if (tmpval == -1)
				return;
			for (int i = 1; i < 4; i++)
				if (tmpval != node->subnode[i]->val)
					return;
			killChildren(node);
			node->val = tmpval;
		}
	}

	int getVal(quad_node *node, int x1, int y1)
	{
		if (node->val != -1)
			return node->val;
		if (x1 <= (node->xmax + node->xmin)/2) {
			if (y1 <= (node->ymax + node->ymin)/2)
				return getVal(node->subnode[0], x1, y1);
			else
				return getVal(node->subnode[1], x1, y1);
		}
		else {
			if (y1 <= (node->ymax + node->ymin)/2)
				return getVal(node->subnode[3], x1, y1);
			else
				return getVal(node->subnode[2], x1, y1);
		}
	}

	void getOverlap(quad_node *node, quad_node *other)
	{
		if (node->val == -1 && other->val == -1) {
			for (int i = 0; i < 4; i++)
				getOverlap(node->subnode[i], other->subnode[i]);
			int tmpval = node->subnode[0]->val;
			if (tmpval == -1)
				return;
			for (int i = 1; i < 4; i++)
				if (tmpval != node->subnode[i]->val)
					return;
			killChildren(node);
			node->val = tmpval;
		}
		else if (node->val == -1) {
			if (other->val == 0)
				return;
			else {
				killChildren(node);
				node->val = 1;
			}
		}
		else if (other->val == -1) {
			if (node->val == 1)
				return;
			else {
				node->val = -1;
				for (int i = 0; i < 4; i++) {
					node->subnode[i] = new quad_node();
					copyQuad(node->subnode[i], other->subnode[i]);
				}
			}
		}
		else {
			if (node->val == 0)
				node->val = other->val;
		}
	}

	void getIntersect(quad_node *node, quad_node *other)
	{
		if (node->val == -1 && other->val == -1) {
			for (int i = 0; i < 4; i++)
				getIntersect(node->subnode[i], other->subnode[i]);
			int tmpval = node->subnode[0]->val;
			if (tmpval == -1)
				return;
			for (int i = 1; i < 4; i++)
				if (tmpval != node->subnode[i]->val)
					return;
			killChildren(node);
			node->val = tmpval;
		}
		else if (node->val == -1) {
			if (other->val == 1)
				return;
			else {
				killChildren(node);
				node->val = 0;
			}
		}
		else if (other->val == -1) {
			if (node->val == 0)
				return;
			else {
				node->val = -1;
				for (int i = 0; i < 4; i++) {
					node->subnode[i] = new quad_node();
					copyQuad(node->subnode[i], other->subnode[i]);
				}
			}
		}
		else {
			if (node->val == 1)
				node->val = other->val;
		}
	}

	void getComplement(quad_node *node)
	{
		if (node->val == 0)
			node->val = 1;
		else if (node->val == 1)
			node->val = 0;
		else
			for (int i = 0; i < 4; i++)
				getComplement(node->subnode[i]);
	}

	void sizeUp(quad_node *node, int m)
	{
		node->xmin *= (1<<(m-n));
		node->xmax = (node->xmax + 1) * (1<<(m-n)) - 1;
		node->ymin *= (1<<(m-n));
		node->ymax = (node->ymax + 1) * (1<<(m-n)) - 1;
		if (node->val == -1)
			for (int i = 0; i < 4; i++)
				sizeUp(node->subnode[i], m);
	}

	long long int numOnes(quad_node *node)
	{
		if (node->val == 0)
			return 0;
		else if (node->val == 1)
			return ((long long int) (node->xmax - node->xmin + 1)) * (node->ymax - node->ymin + 1);
		else {
			long long int ans = 0;
			for (int i = 0; i < 4; i++)
				ans += numOnes(node->subnode[i]);
			return ans;
		}
	}

	void sizeDown(quad_node *node, int m)
	{
		if (node->val != -1) {
			node->xmin /= (1<<(n-m));
			node->xmax = (node->xmax + 1)/(1<<(n-m)) - 1;
			node->ymin /= (1<<(n-m));
			node->ymax = (node->ymax + 1)/(1<<(n-m)) - 1;
		}
		else if (node->xmax - node->xmin + 1 == (1<<(n-m))) {
			long long int num1s = numOnes(node);
			long long int comp = 1;
			comp <<= (2*(n-m)-1);
			killChildren(node);
			node->xmin /= (1<<(n-m));
			node->xmax = node->xmin;
			node->ymin /= (1<<(n-m));
			node->ymax = node->ymin;
			if (num1s >= comp)
				node->val = 1;
			else
				node->val = 0;
		}
		else {
			node->xmin /= (1<<(n-m));
			node->xmax = (node->xmax + 1)/(1<<(n-m)) - 1;
			node->ymin /= (1<<(n-m));
			node->ymax = (node->ymax + 1)/(1<<(n-m)) - 1;
			for (int i = 0; i < 4; i++)
				sizeDown(node->subnode[i], m);
			int tmpval = node->subnode[0]->val;
			if (tmpval == -1)
				return;
			for (int i = 1; i < 4; i++)
				if (tmpval != node->subnode[i]->val)
					return;
			killChildren(node);
			node->val = tmpval;
		}
	}

	void getExtract(quad_node *node, quad_node *other, int x1, int y1, int m)
	{
		if (node->val != -1) {
			other->val = node->val;
			return;
		}
		else if (x1 + (1<<m) - 1 <= (node->xmin + node->xmax)/2) {
			if (y1 + (1<<m) - 1 <= (node->ymin + node->ymax)/2) {
				getExtract(node->subnode[0], other, x1, y1, m);
				return;
			}
			else if (y1 > (node->ymin + node->ymax)/2) {
				getExtract(node->subnode[1], other, x1, y1, m);
				return;
			}
		}
		else if (x1 > (node->xmin + node->xmax)/2) {
			if (y1 + (1<<m) - 1 <= (node->ymin + node->ymax)/2) {
				getExtract(node->subnode[3], other, x1, y1, m);
				return;
			}
			else if (y1 > (node->ymin + node->ymax)/2) {
				getExtract(node->subnode[2], other, x1, y1, m);
				return;
			}
		}
		for (int i = 0; i < 4; i++) {
			other->subnode[i] = new quad_node();
			if (i <= 1) {
				other->subnode[i]->xmin = other->xmin;
				other->subnode[i]->xmax = (other->xmax + other->xmin)/2;
			}
			else {
				other->subnode[i]->xmin = (other->xmax + other->xmin)/2 + 1;
				other->subnode[i]->xmax = other->xmax;
			}
			if (i == 0 || i == 3) {
				other->subnode[i]->ymin = other->ymin;
				other->subnode[i]->ymax = (other->ymax + other->ymin)/2;
			}
			else {
				other->subnode[i]->ymin = (other->ymax + other->ymin)/2 + 1;
				other->subnode[i]->ymax = other->ymax;
			}
		}
		other->val = -1;
		getExtract(node, other->subnode[0], x1, y1, m-1);
		getExtract(node, other->subnode[1], x1, y1 + (1<<(m-1)), m-1);
		getExtract(node, other->subnode[2], x1 + (1<<(m-1)), y1 + (1<<(m-1)), m-1);
		getExtract(node, other->subnode[3], x1 + (1<<(m-1)), y1, m-1);
		int tmpval = other->subnode[0]->val;
		if (tmpval == -1)
			return;
		for (int i = 1; i < 4; i++)
			if (tmpval != other->subnode[i]->val)
				return;
		killChildren(other);
		other->val = tmpval;
	}

	public:

		quad_tree(int n)
		{
			this->n = n;
			root = new quad_node();
			root->val = 0;
			root->xmin = root->ymin = 0;
			root->xmax = root->ymax = (1<<n)-1;
		}

		~quad_tree()
		{
			killChildren(root);
			delete root;
		}

		quad_tree(quad_tree const &Q)
		{
			root = new quad_node();
			copyQuad(root, Q.root);
			n = Q.n;
		}

		void set(int x1, int y1, int x2, int y2, int b)
		{
			setVal(root, x1, y1, x2, y2, b);
		}

		int get(int x1, int y1)
		{
			return getVal(root, x1, y1);
		}

		int size() const
		{
			return n;
		}

		void overlap(quad_tree const &Q)
		{
			getOverlap(root, Q.root);
		}

		void intersect(quad_tree const &Q)
		{
			getIntersect(root, Q.root);
		}

		void complement()
		{
			getComplement(root);
		}

		void resize(int m)
		{
			if (m == n)
				return;
			else if (m > n)
				sizeUp(root, m);
			else
				sizeDown(root, m);
			n = m;
		}

		void extract(int x1, int y1, int m)
		{
			quad_node *other = new quad_node();
			copyQuad(other, root);
			killChildren(root);
			root->val = 0;
			root->xmin = root->ymin = 0;
			root->xmax = root->ymax = (1<<m)-1;
			getExtract(other, root, x1, y1, m);
			n = m;
			killChildren(other);
			delete other;
		}
};