#pragma once
# ifndef RB_BASE_HPP
# define RB_BASE_HPP

namespace ft {

	enum	Color { _red = false, _black = true };

	template <typename Val>
	struct	Rb_Node {

		typedef Rb_Node<Val>*		Node_ptr;
		typedef	const Rb_Node<Val>*	Const_Node_ptr;

		Color		color;
		Node_ptr	parent;
		Node_ptr	left;
		Node_ptr	right;
		Val			value;

		static Node_ptr	minimum(Node_ptr x) {
			while  (x->left != 0) x = x->left;
			return x;
		}

		static Const_Node_ptr	minimum(Const_Node_ptr x) {
			while  (x->left != 0) x = x->left;
			return x;
		}

		static Node_ptr	maximum(Node_ptr x) {
			while  (x->right != 0) x = x->right;
			return x;
		}

		static Const_Node_ptr	maximum(Const_Node_ptr x) {
			while  (x->right != 0) x = x->right;
			return x;
		}
	};

}//namespace

#endif