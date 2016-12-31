using System;
using System.Text;
using System.Collections.Generic;

namespace bioinf_sufix_array
{
	class MainClass
	{
		public const bool LTYPE = true;
		public const bool STYPE = false;
		public const bool BUCKET_END = true;
		public const bool BUCKET_START = false;

		public const int d = 2;

		//public static string test = "ATGCAAAATTTCCGGGGGAAATTTCCGGGGAAAATTT$";
		public static string test = "mmiissiissiippii$";

		public static void Main (string[] args)
		{
			int[] sa = new int[test.Length];
			SA_DS (Encoding.ASCII.GetBytes(test), sa, 255, 0);
		}

		public static void SA_DS (byte[] s, int[] sa, int alphabetSize, int recursionLevel)
		{
			bool[] t = new bool[s.Length];

			mapLSType (s, t);

			int[] P1 = new int[s.Length];

			int n1 = getDCriticalPointers(s, t, P1);

			int[] B = getBuckets (s, alphabetSize, BUCKET_END);

//			for (int i = 0; i < sa.Length; i++) {
//				sa [i] = -1;
//			}
//
			// FIRST  STEP
			for (int i = 0; i < n1; i++) {
				sa[--B [s [P1 [i]]]] = P1[i];
			}

			Console.Out.WriteLine ("First step: SA: " + String.Join (" ", sa));
//
//			B = getBuckets (s, alphabetSize, BUCKET_START);
//
//			// SECOND STEP
//			for (int i = 0; i < sa.Length; i++) {
//				if (sa [i] > 0) {
//					if (t [sa [i] - 1] == LTYPE) {
//						sa [B [s [sa [i] - 1]]++] = sa [i] - 1;
//					}
//				}
//			}
//
//			Console.Out.WriteLine ("Second step: SA: " + String.Join (" ", sa));

			int[] b = new int[n1];
			int[] a = P1;

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


			// NAMING
//			int[] tmp = new int[s.Length];
//			for (int i = 0; i < s.Length - 1; i++) {
//				tmp [i] = - 1;
//			}
//
//			for (int i = 0; i < n1; i++) {
//				tmp[P1[i]] = 
//			}
			int[] c = new int[alphabetSize + 1];
			for (int i = 0; i < s.Length; i++) {
				c [s [i]]++;
			}

			for (int i = 0, sum = 0; i < alphabetSize + 1; i++) {
				int r = c [i]; c [i] = sum; sum += r; 
			}

			Console.Out.WriteLine("Buckets: " + String.Join(" ", c));
			//for (int i = 0









			//DEBUG P1
			Console.Out.WriteLine ("S:" + s);
			Console.Out.Write ("t:");
			for (int i = 0; i < t.Length; i++) {
				if (t [i] == LTYPE) {
					Console.Out.Write ("L");
				} else {
					Console.Out.Write ("S");
				}
			}
			Console.Out.WriteLine ();
			Console.Out.WriteLine ("P1:" + string.Join(" ", P1));




	//		for (int i = 0; i < sa.Length; i++) {
	//			sa [i] = -1;
	//		}



		}



		public static void SA_DS (int[] s, int[] sa, int alphabetSize, int recursionLevel) {
		
		}


		public static int[] getBuckets(byte[] s, int alphabetSize, bool getEnds) {
			int[] bucket = new int[alphabetSize];
			for (int i = 0; i < s.Length; i++) {
				bucket [s [i]]++;
			}
			int sum = 0;
			for (int i = 0; i < alphabetSize; i++) {
				sum += bucket [i]++;
				bucket [i] = getEnds ? sum : sum - bucket [i];
			}
			return bucket;
		}

		public static int getDCriticalPointers(byte[] s, bool[] t, int[] P1) {
			int k = 0;
			for (int i = 1; i < s.Length; i++) {
				if (t [i] == STYPE && t [i - 1] == LTYPE) {
					P1 [k++] = i;
				} else if ((i >= d && i < s.Length - 1) && !(t [i + 1] == STYPE && t [i] == LTYPE)) {
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

		public static void mapLSType(byte[] s, bool[] t) {
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


		public static void bucketSort(int[] a, int[] b, byte[] s, int n1, int alphabetSize, int d) {

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





