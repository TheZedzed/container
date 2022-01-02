#pragma once
#ifndef RB_ALGO_HPP
# define RB_ALGO_HPP

# include "Rb_base.hpp"

namespace ft {

	template <typename Val>
	static void	rotate_left(Rb_Node<Val>* const x, Rb_Node<Val>*& root) {
		Rb_Node<Val>* const	y = x->right;

		x->right = y->left;
		if (y->left != 0)
			y->left->parent = x;
		y->parent = x->parent;
		if (x == root)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	template <typename Val>
	static void	rotate_right(Rb_Node<Val>* const x, Rb_Node<Val>*& root) {
		Rb_Node<Val>* const	y = x->left;

		x->left = y->right;
		if (y->right != 0)
			y->right->parent = x;
		y->parent = x->parent;
		if (x == root)
			root = y;
		else if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}

	template <typename Val>
	void	insert_and_rebalance(const bool insertleft, Rb_Node<Val>* x, Rb_Node<Val>* p, Rb_Node<Val>& header) {
		Rb_Node<Val>*&	root = header.parent;

		// Initialize fields in new node to insert.
		x->parent = p;
		x->left = 0;
		x->right = 0;
		x->color = _red;
		// Insert.
		// Make new node child of parent and maintain root, leftmost and
		// rightmost nodes.
		// N.B. First node is always inserted left.
		if (insertleft) {
			p->left = x; // also makes leftmost = __x when __p == &__header
			if (p == &header) {
				header.parent = x;
				header.right = x;
			}
			else if (p == header.left)
				header.left = x; // maintain leftmost pointing to min node
		}
		else {
			p->right = x;
			if (p == header.right)
				header.right = x; // maintain rightmost pointing to max node
		}
		// Rebalance.
		while (x != root && x->parent->color == _red) {
			Rb_Node<Val>* const	xpp = x->parent->parent;

			if (x->parent == xpp->left) {
				Rb_Node<Val>* const	y = xpp->right;
				if (y && y->color == _red) {
					x->parent->color = _black;
					y->color = _black;
					xpp->color = _red;
					x = xpp;
				}
				else {
					if (x == x->parent->right) {
						x = x->parent;
						rotate_left(x, root);
					}
					x->parent->color = _black;
					xpp->color = _red;
					rotate_right(xpp, root);
				}
			}
			else {
				Rb_Node<Val>* const	y = xpp->left;
				if (y && y->color == _red) {
					x->parent->color = _black;
					y->color = _black;
					xpp->color = _red;
					x = xpp;
				}
				else {
					if (x == x->parent->left) {
						x = x->parent;
						rotate_right(x, root);
					}
					x->parent->color = _black;
					xpp->color = _red;
					rotate_left(xpp, root);
				}
			}
		}
		root->color = _black;
	}

	template <typename Val>
	Rb_Node<Val>*	rebalance_for_erase(Rb_Node<Val>* const z, Rb_Node<Val>& header) {
		Rb_Node<Val>*&	root = header.parent;
		Rb_Node<Val>*&	leftmost = header.left;
		Rb_Node<Val>*&	rightmost = header.right;
		Rb_Node<Val>*	y = z;
		Rb_Node<Val>*	x = 0;
		Rb_Node<Val>*	xparent = 0;

		if (y->left == 0)     // __z has at most one non-null child. y == z.
			x = y->right;     // __x might be null.
		else {
			if (y->right == 0)  // __z has exactly one non-null child. y == z.
				x = y->left;    // __x is not null.
			else {
				// z has two non-null children.  Set __y to
				y = y->right;   //   __z's successor.  __x might be null.
				while (y->left != 0)
					y = y->left;
				x = y->right;
			}
		}
		if (y != z) {
			// relink y in place of z.  y is z's successor
			z->left->parent = y;
			y->left = z->left;
			if (y != z->right) {
				xparent = y->parent;
				if (x)
					x->parent = y->parent;
				y->parent->left = x;   // __y must be a child of _Mleft
				y->right = z->right;
				z->right->parent = y;
			}
			else
				xparent = y;
			if (root == z)
				root = y;
			else if (z->parent->left == z)
				z->parent->left = y;
			else
				z->parent->right = y;
			y->parent = z->parent;
			std::swap(y->color, z->color);
			y = z;
			// __y now points to node to be actually deleted
		}
		else {//y == z
			xparent = y->parent;
			if (x)
				x->parent = y->parent;
			if (root == z)
				root = x;
			else {
				if (z->parent->left == z)
					z->parent->left = x;
				else
					z->parent->right = x;
			}
			if (leftmost == z) {
				if (z->right == 0)
					leftmost = z->parent;
				else
					leftmost = Rb_Node<Val>::minimum(x);
			}
			if (rightmost == z) {
				if (z->left == 0)
					rightmost = z->parent;
				else
					rightmost = Rb_Node<Val>::maximum(x);
			}
		}
		if (y->color != _red) {
			while (x != root && (x == 0 || x->color == _black)) {
				if (x == xparent->left) {
					Rb_Node<Val>*	w = xparent->right;
					if (w->color == _red) {
						w->color = _black;
						xparent->color = _red;
						rotate_left(xparent, root);
						w = xparent->right;
					}
					if ((w->left == 0 || w->left->color == _black)
						&& (w->right == 0 || w->right->color == _black)) {
						w->color = _red;
						x = xparent;
						xparent = xparent->parent;
					}
					else {
						if (w->right == 0 || w->right->color == _black) {
							w->left->color = _black;
							w->color = _red;
							rotate_right(w, root);
							w = xparent->right;
						}
						w->color = xparent->color;
						xparent->color = _black;
						if (w->right)
							w->right->color = _black;
						rotate_left(xparent, root);
						break ;
					}
				}
				else {
					// same as above, with _Mright <-> _Mleft.
					Rb_Node<Val>*	w = xparent->left;
					if (w->color == _red) {
						w->color = _black;
						xparent->color = _red;
						rotate_right(xparent, root);
						w = xparent->left;
					}
					if ((w->right == 0 || w->right->color == _black)
						&& (w->left == 0 || w->left->color == _black)) {
						w->color = _red;
						x = xparent;
						xparent = xparent->parent;
					}
					else {
						if (w->left == 0 || w->left->color == _black) {
							w->right->color = _black;
							w->color = _red;
							rotate_left(w, root);
							w = xparent->left;
						}
						w->color = xparent->color;
						xparent->color = _black;
						if (w->left)
							w->left->color = _black;
						rotate_right(xparent, root);
						break ;
					}
				}
			}
			if (x) x->color = _black;
		}
		return y;
	}

}//namespace

#endif