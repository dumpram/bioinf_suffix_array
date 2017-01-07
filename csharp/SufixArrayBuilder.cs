using System;
using System.Text;
using System.Collections.Generic;

namespace bioinf_sufix_array
{
	class MainClass
	{
		public const bool LTYPE = false;
		public const bool STYPE = true;
		public const bool BUCKET_END = true;
		public const bool BUCKET_START = false;

		public const int d = 2;

		//public static string test = "ATGCAAAATTTCCGGGGGAAATTTCCGGGGAAAATTT$";
		public static string test = "mmiissiissiippii$";

		public static void Main (string[] args)
		{
			int[] sa = new int[test.Length];
			byte[] b = Encoding.ASCII.GetBytes (test);
			int[] s = new int[b.Length];
			for (int i = 0; i < b.Length; i++) {
				s [i] = (int)b [i];
			}
			SA_DS (s, sa, 255, 0);
		}

		public static void SA_DS (int[] s, int[] sa, int alphabetSize, int recursionLevel)
		{
			bool[] t = new bool[s.Length];
			mapLSType (s, t);
			int[] P1 = new int[s.Length];
			int n1 = getDCriticalPointers(s, t, P1);

			int[] b = new int[n1];
			int[] a = P1;

			byte[] sw = new byte[s.Length];
			for (int i = 0; i < sw.Length; i++) {
				if (t [i]) {
					sw [i] = (byte)(2 * s [i] + 1);
				} else {
					sw [i] = (byte)(2 * s [i]);
				}
			}

			int[] P_1 = new int[n1];
			for (int i = 0; i < n1; i++) {
				P_1 [i] = P1 [i];
			}

			Console.Out.WriteLine ("Sw: " + String.Join (" ", sw));

			Console.Out.WriteLine ("P1: " + String.Join (" ", P1));

			for (int i = 0; i < d + 2; i++) {
				if (i % 2 == 0) {
					bucketSort (a, b, s, n1, alphabetSize, d + 1 - i);
					P1 = b;
				} else {
					bucketSort (b, a, s, n1, alphabetSize, d + 1 - i);
					P1 = a;
				}
			}
			Console.Out.WriteLine ("Complete pass: " + String.Join (" ", P1));
			for (int i = 0; i < t.Length; i++) {
				if (t [i] == LTYPE) {
					Console.Out.Write ("L");
				} else {
					Console.Out.Write ("S");
				}
			}
			Console.Out.WriteLine ();
			int name = 0;
	
			int[] S1 = new int[n1];
			bool diff = false;
			for (int i = 0; i < n1; i++) {
				if (i == 0) {
					S1 [0] = name;
					continue;
				}
				diff = false;
				for (int j = 0; j < d + 2; j++) {
					if (s [P1 [i - 1] + j] != s [P1 [i] + j]) {
						diff = true;
						S1 [i] = ++name;
						break;
					}
				}
				if (!diff) {
					S1 [i] = name;
				}
			}


			int[] tmp = new int[s.Length];
			for (int i = 0; i < tmp.Length; i++) {
				tmp [i] = -1;
			}
			for (int i = n1 - 1; i >= 0; i--) {
				tmp [P1 [i]] = S1 [i];
			}
			for (int i = 0, k = 0; i < s.Length; i++) {
				if (tmp [i] != -1) {
					S1 [k++] = tmp [i];
				}
			}
			Console.Out.WriteLine ("S1: " + String.Join (" ", S1));

			int[] SA1 = new int[S1.Length];

		
			if (name < n1 - 1) {
				SA_DS (S1, SA1, name + 1, recursionLevel + 1);
			} else {
				SA1 = new int[S1.Length];
				//induce SA1 from S1
				Console.Out.WriteLine ("Inducing SA1 from S1");
				//Console.Out.WriteLine ("B: " + String.Join(" ", B) + " " +  alphabetSize);
				for (int i = 0; i < SA1.Length; i++) {
					SA1 [S1 [i]] = i;
				}
				Console.Out.WriteLine ("SA1: " + String.Join (" ", SA1));
			}
			//induce SA from SA1
			Console.Out.WriteLine ("Inducing SA from SA1: " + String.Join(" ", SA1));

			int[] SA = new int[s.Length];
			for (int i = 0; i < SA.Length; i++) {
				SA[i] = -1;
			}
			int[] B = getBuckets (s, alphabetSize, BUCKET_END);

			for (int i = SA1.Length - 1; i >= 0; i--) {
				// check if is LMS and add to end of the bucket
				if (P_1[SA1[i]] > 0 && t[P_1[SA1[i]]] == STYPE && t[P_1[SA1[i]]-1] == LTYPE) {
					SA [--B [s [P_1[SA1 [i]]]]] = P_1 [SA1 [i]];
				}
			}

			Console.Out.WriteLine ("Step 1: " + String.Join (" ", SA));


			B = getBuckets (s, alphabetSize, BUCKET_START);
			for (int i = 0; i < SA.Length; i++) {
				if (SA [i] > 0) {
					if (t [SA [i] - 1] == LTYPE) {
						SA[++B[s[SA[i] - 1]]] = SA[i] - 1;
					}
				}
			}

			Console.Out.WriteLine ("Step 2: " + String.Join (" ", SA));

			B = getBuckets (s, alphabetSize, BUCKET_END);
			for (int i = SA.Length - 1; i >= 0; i--) {
				if (SA [i] > 0) {
					if (t [SA [i] - 1] == STYPE) {
						SA[--B[s[SA[i] - 1]]] = SA[i] - 1;
					}
				}
			}
			Console.Out.WriteLine ("Step 3: " + String.Join (" ", SA));

			for (int i = 0; i < sa.Length; i++) {
				sa [i] = SA [i];
			}
			Console.Out.WriteLine ("SA: " + String.Join (" ", sa));
		}




//		public static void SA_DS (int[] s, int[] sa, int alphabetSize, int recursionLevel) {
			
//		}


		public static int[] getBuckets(int[] s, int alphabetSize, bool getEnds) {
			int[] bucket = new int[alphabetSize];
			for (int i = 0; i < s.Length; i++) {
				bucket [s [i]]++;
			}
			int sum = 0;
			for (int i = 0; i < alphabetSize; i++) {
				sum += bucket [i];
				bucket [i]++;
				bucket [i] = getEnds ? sum : sum - bucket [i];
			}
			return bucket;
		}

		public static int getDCriticalPointers(int[] s, bool[] t, int[] P1) {
			int k = 0;
			for (int i = 1; i < s.Length; i++) {
				if (t [i] == STYPE && t [i - 1] == LTYPE) {
					P1 [k++] = i;
				} else if ((i >= d && i < s.Length - 1) && !(t[i + 1] == STYPE && t [i] == LTYPE)) {
					if (k > 0 && P1 [k - 1] == i - d) {
						P1 [k++] = i;
					}
				}
			}
			return k;
		}

		public int[] getBuckets(int[] s, bool getEnds) { 
			return null;
		}

		public static void mapLSType(int[] s, bool[] t) {
			for (int i = s.Length - 1; i >= 0; i--) {
				if (i == s.Length - 1) {
					t [i] = STYPE;
				} else if (s [i] > s [i + 1] || s [i] == s [i + 1] && t [i + 1] == LTYPE) {
					t [i] = LTYPE;
				} else {
					t [i] = STYPE;
				}
			}
		}


		public static void bucketSort(int[] a, int[] b, int[] s, int n1, int alphabetSize, int d) {

			int[] c = new int[alphabetSize + 1];

			for (int i = 0, j = 0; i < n1; i++) {
				j = a [i] + d;
				j = (j > s.Length - 1) ? s.Length - 1 : j;
				c [s[j]]++;
			}

			for (int i = 0, sum = 0; i < alphabetSize + 1; i++) {
				int t = c [i]; c [i] = sum; sum += t; 
			}

			for (int i = 0, j = 0; i < n1; i++) {
				j = a [i] + d;
				j = (j > s.Length - 1) ? s.Length - 1 : j;
				b[c[s [j]]++] = a[i];
			}
		}

		public static int[] getBucketBySiblings(int[] a, byte[] s, int n1, int alphabetSize, int d) {
			int[] c = new int[alphabetSize + 1];

			for (int i = 0, j = 0; i < n1; i++) {
				j = a [i] + d;
				j = (j > s.Length - 1) ? s.Length - 1 : j;
				c [s[j]]++;
			}

			for (int i = 0, sum = 0; i < alphabetSize + 1; i++) {
				int t = c [i]; c [i] = sum; sum += t; 
			}
			return c;
		}


		public static void bucketSortLS(int[] a, int[] b, byte[] s, bool[] t,  int n1, int d) {

			int[] c = {0, n1 - 1};

			for (int i = 0, j = 0;  i < n1; i++) {
				j = a[i] +  d;
				j = (j > s.Length - 1) ? s.Length - 1 : j;
				if (t[j] == STYPE) { 
					b[c[1]--] = a[i];
				} else {
					b[c[0]++] = a[i];
				}
			}

		}
	}
}





